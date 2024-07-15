/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:20 by marvin            #+#    #+#             */
/*   Updated: 2024/07/04 15:06:30 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

volatile sig_atomic_t isRunning = 1;

Server::Server() : _port("6667"), _password("password") {}

Server::Server(char **av) : _port(av[1]), _password(av[2]) {}

Server::Server(Server const &obj) { *this = obj; }

Server& Server::operator=(Server const &obj) {
    if (this == &obj)
        return (*this);
    return(*this);
}

Server::~Server() {
	for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); ++it) {
		delete it->second;
	}
	for (std::map<int, Client*>::iterator it = this->_tmpClients.begin(); it != this->_tmpClients.end(); ++it) {
		delete it->second;
	}
	this->_tmpClients.clear();
}

std::string Server::getPassword() { return ( this->_password ); }
std::string Server::getPort() { return ( this->_port ); }
int 		Server::getSocketFdBot() { return ( this->_socketFdBot ); }

uint16_t Server::getPortAsUint16() {
	char			*endptr;
	unsigned long	port_ul = strtoul(this->_port.c_str(), &endptr, 10);

	if (*endptr != '\0') {
		std::cerr << "Invalid port number format" << std::endl;
		return (0);
	}
	return (static_cast<uint16_t>(port_ul));
}

int Server::createUser() {
	return (socket(AF_INET, SOCK_STREAM, 0));
}

int Server::bindUser(int sockfd, const struct sockaddr_in& server_addr) {
	if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Failed to bind socket" << std::endl;
		return (1);
	}
	return (0);
}

int Server::listenUser(int sockfd) {
	if (listen(sockfd, SOMAXCONN) < 0) {
		std::cerr << "Failed to listen on socket" << std::endl;
		return (1);
	}
	return (0);
}

void Server::handleNewConnection(int epoll_fd, int sockfd) {
	struct sockaddr_in	clientAddr;
	struct epoll_event	event;
	socklen_t			clientAddrLen;
	int					clientSocket;

	clientAddrLen = sizeof(clientAddr);
	clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);

	if (clientSocket == -1) {
		std::cerr << "Failed to accept client connection: " << strerror(errno) << std::endl;
		return ;
	}
	_tmpClients[clientSocket] = new Client();
	_tmpClients[clientSocket]->setSocketFD(clientSocket);

	std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = clientSocket;

	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
		std::cerr << "Failed to add client socket to epoll: " << strerror(errno) << std::endl;
		close(clientSocket);
		return ;
	}
}

void Server::handleClientData(int clientSocket) {
	char												buffer[1024];
	int													bytesRead;
	std::string											line;
	std::vector<std::string>							tokens;
	std::pair<std::vector<std::string>, std::string>	input;

	bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
	buffer[bytesRead] = '\0';
	_messages[clientSocket] += buffer;

	while (_messages[clientSocket].find("\n") != std::string::npos)
	{
		line = _messages[clientSocket].substr(0, _messages[clientSocket].find("\n"));
//		if (!line.size()) {
//			std::cout << "Client Disconnected\n";  //in case something goes wrong recheck this again
//			break ;
//		}
		_messages[clientSocket] = _messages[clientSocket].substr(_messages[clientSocket].find("\n") + 1);

		if (line.find("\r") != std::string::npos)
			line.erase(line.find("\r"));
		initInput(&input, line);
		printInput(input, _tmpClients[clientSocket]);
		tokens = split(line, ' ');
		if (!tokens.empty()) {
			if (_tmpClients[clientSocket] == NULL)
				return ;
			bool check = _tmpClients[clientSocket]->checkClientParams(*this, line);
			if (_tmpClients[clientSocket]->getValidData() && checkForOperators(line, _tmpClients[clientSocket], input)) {
				std::cout << "comando executado\n";
				break ; // if something goes wrong DELETE
			}
			else if (!check && tokens[0] != "CAP") {
				std::cerr << "Invalid command" << std::endl;
				sendMessage(clientSocket, "ERROR :Invalid command\n");
			}
		}
	}
}

void signalHandler(int signal) {
	if (signal == SIGINT) {
		std::cout << "\nCtrl+C detected. Shutting Down Server..." << std::endl;
		isRunning = 0;
	}
	if (signal == SIGPIPE) {
		std::cout << "Forced Shutdown.\n" << std::endl;
		isRunning = 1;
	}
}

int Server::epollFunction() {
	int					sockfd;
	int					opt;
	int					epoll_fd;
	int					flags;
	int					numEvents;
	struct epoll_event	event;
	struct sockaddr_in	server_addr;

	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(getPortAsUint16());
	server_addr.sin_addr.s_addr = INADDR_ANY;
	opt = 1;
	sockfd = createUser();
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
		return (1);
	if (bindUser(sockfd, server_addr))
		return (1);
	if (listenUser(sockfd))
		return (1);

	struct sigaction sa;
	sa.sa_handler = signalHandler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;
	sigaction(SIGINT, &sa, NULL);
	sigaction(SIGPIPE, &sa, NULL);


	epoll_fd = epoll_create1(0);

	if (epoll_fd == -1) {
		std::cerr << "Epoll creation failed" << std::endl;
		return (1);
	}

	flags = fcntl(sockfd, F_GETFL, 0);
	fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);

	event.events = EPOLLIN | EPOLLET;
	event.data.fd = sockfd;
	if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
		std::cerr << "Failed to add server socket to epoll" << std::endl;
		return (1);
	}

	std::cout << "Server started. Listening on port " << getPort() << "..." << std::endl;

	createBot("127.0.0.1", 6667);
	epoll_event events[MAX_EVENTS];
	//sendMessage(sockfd, "CHANLIMIT=#&:25\r\n");
	while (isRunning) {
		numEvents = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
		for (int i = 0; i < numEvents; ++i) {
			if (events[i].data.fd == sockfd) {
				handleNewConnection(epoll_fd, sockfd);
			} else {
				handleClientData(events[i].data.fd);
			}
		}
	}
	close(sockfd);
	close(_socketFdBot);
	close(epoll_fd);
	return (0);
}

void	Server::sendMessage(int fd, std::string message) {
	const char	*buffer = message.c_str();
	send(fd, buffer, std::strlen(buffer), MSG_DONTWAIT);
}

bool	privmsgChecker(std::vector<std::string> args) {
	if (args[0][0] != '#' && args[0][0] != '&')
		return (true);
	return (false);
}

void Server::sendChannelMessage(std::pair<std::vector<std::string>, std::string> input, Client* user) {
	std::vector<std::string> args;
	getArgsPro(&args, input, 1);

	std::map<int, Client*>::iterator it2;
	if (privmsgChecker(args)) {
		for (it2 = _tmpClients.begin(); it2 != _tmpClients.end(); ++it2) {
			if (it2->second->getNick() == args[0]) {
				break;
			}
		}
		if (it2 == _tmpClients.end()){
			std::string msgNoSuchNick = ":" + displayHostname() + " 401 " + user->getNick() + " " + args[0] + " :No such nick\r\n";
			sendMessage(user->getSocketFD(), msgNoSuchNick);
			return ;
		}
		std::string msgPrivate = ":" + user->getNick() + " PRIVMSG " + args[0] + " :" + args[1] + "\r\n";
		sendMessage(it2->second->getSocketFD(), msgPrivate);
		return ;
	}

	std::map<std::string, Channel*>::iterator it = _channelsList.find(args[0]);
	if (it == _channelsList.end()) {
		std::string msgNoSuchChannel = ":" + displayHostname() + " 401 " + user->getNick() + " " + args[0] + " :No such channel\r\n";
		sendMessage(user->getSocketFD(), msgNoSuchChannel);
		return;
	}


	std::vector<int> membersFd = it->second->getMembersFd();
	for (std::vector<int>::iterator itt = membersFd.begin(); itt != membersFd.end(); ++itt) {
		if (user->getSocketFD() != *itt) {
			sendMessage(*itt, PRIVMSG(user->getNick(), args[0], args[1]));
		}
		if (args[1] == "BOT time") {
			std::time_t now = std::time(NULL);
			std::tm* localTime = std::localtime(&now);
			char timeBuffer[6];
			std::strftime(timeBuffer, sizeof(timeBuffer), "%H:%M", localTime);
			std::string msgReplyBot = "The time right now is " + std::string(timeBuffer);
			sendMessage(*itt, PRIVMSG("BOT", args[0], msgReplyBot));
		}
		else if (args[1] == "BOT joke") {
			sendMessage(*itt, PRIVMSG("BOT", args[0], BotJokes()));
		}
	}
}

bool Server::checkUniqueNick(const std::string& toCheck) {
	for (std::map<int,Client *>::iterator it=this->_tmpClients.begin(); it!=this->_tmpClients.end(); ++it){
		if(!toCheck.compare(it->second->getNick()))
			return (false);
	}
	return (true);
}

Client *Server::_getUserClass(const std::string& userNick) {
	std::map<int,Client *>::iterator it;
	for (it = _tmpClients.begin(); it != _tmpClients.end(); ++it) {
		if(it->second->getNick() == userNick)
			return (it->second);
	}
	return (NULL);
}
