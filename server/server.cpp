/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:20 by marvin            #+#    #+#             */
/*   Updated: 2024/07/01 19:00:08 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() : _port("6667"), _password("password"), _clientInfo() {}
Server::Server(char **av) : _clientInfo() {
    this->_port = av[1];
    this->_password = av[2];
}

Server::Server(const Server &obj) {
    (void)obj;
}

Server& Server::operator=(const Server &obj) {
    (void)obj;
    return(*this);
}

Server::~Server() {
	for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); ++it) {
		delete it->second;
	}
}

std::string Server::getPassword() { return ( this->_password ); }
std::string Server::getPort() { return ( this->_port ); }


uint16_t Server::getPortAsUint16() {
    char* endptr;
    unsigned long port_ul = strtoul(this->_port.c_str(), &endptr, 10);
    if (*endptr != '\0') {
        std::cerr << "Invalid port number format" << std::endl;
        return 0;
    }
    return static_cast<uint16_t>(port_ul);
}

int Server::createUser() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

int Server::bindUser(int sockfd, const struct sockaddr_in& server_addr) {
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return 1;
    }
    return 0;
}

int Server::listenUser(int sockfd) {
    if (listen(sockfd, SOMAXCONN) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return 1;
    }
    return 0;
}

void clearCharPointer(char* str) {
    if (str) {
        std::memset(str, '\0', std::strlen(str));
    }
}

/* void Server::fillInfo(clientInfo& clientInfo) {
    char buffer[1024];
    recv(clientInfo.socket_fd, buffer, sizeof(buffer) - 1, 0);
    std::vector<std::string> something = split(std::string(buffer), '\n');
    std::vector<std::string> another = split(something.at(0), ' ');
    if (another.at(0) == "NICK"){
        clientInfo.nick = another.at(1);
    } else if (another.at(0) == "PASS") {
        clientInfo.pass = another.at(1);
     } else if (another.at(0) == "USER") {
        clientInfo.user = another.at(1);
    }
    clearCharPointer(buffer);
    another.clear();
    something.clear();
} */

// input
void ft_printargs(std::vector<std::string> args, std::string type)
{
    std::cout << type << ": ";
    for (size_t i = 0; i < args.size(); i++)
    {
        if (i > 0)
            std::cout << " ";
        std::cout << "\"" << args[i] << "\"";
    }
    std::cout << std::endl;
}

void ft_printinput(std::pair<std::vector<std::string>, std::string> input)
{
    std::cout << "------ Input ------\n";
    ft_printargs(input.first, "args");
    std::cout << "text: \"" << input.second << "\"" << std::endl << std::endl;
}

size_t ft_findstr(std::string str)
{
    size_t it = 0, start, end;
    while (str[it] != '\0')
    {
        while (str[it] != '\0' && std::isspace(str[it]) != 0)
            it++;
        start = it;
        while (str[it] != '\0' && std::isspace(str[it]) == 0)
            it++;
        end = it;
        if (end > start)
        {
            std::string tmp = str.substr(start, end - start);
            if (tmp[0] != ':')
                continue ;
            return (start);
        }
        while (str[it] != '\0' && std::isspace(str[it]) != 0)
            it++;
    }
    return (std::string::npos);
}

void    Server::initInput(std::pair<std::vector<std::string>, std::string>* input, std::string line)
{
    // std::cout << "line: $" << line << "$\n";
    input->first = splitspace(line);
    bool message = false;
    size_t len = input->first.size(), i;
    for (i = 0; i < len; i++)
        if (input->first[i].find(":") == 0)
            break ;
    for (; i < len; i++)
    {
        message = true;
        input->first.pop_back();
    }
    if (message == true)
    {
        size_t pos = ft_findstr(line);
        input->second = line.substr(pos + 1, line.size() - (pos + 1));
    }
}
// input

int Server::checkSingle(clientInfo* user, std::string line) {
    std::string clientPassword;
    std::string serverPassword = getPassword();
    std::string content(line);
    std::vector<std::string> tmp = split(line, ' ');
    if (tmp[0] == "PASS")
    {
        if (tmp[1].empty())
        {
            std::cout << "Required Password!" << std::endl;
            return 1;
        }
        if (tmp[1] != serverPassword)
        {
            std::cout << "Wrong Password!" << std::endl;
            return 1;
        }
        user->pass = tmp[1];
        serverPassword.clear();
    }
    else if (tmp[0] == "NICK") {
        user->nick = tmp[1];
    }
    else if (tmp[0] == "USER") {
        user->user = tmp[1];
    }
    return 0;
}

int Server::check_message(clientInfo* client_info, std::string line) {
    if (line.find("\r") != std::string::npos)
        line.erase(line.find("\r"));
    int res = checkSingle(client_info, line);
    if (res == 1)
        return 1;
    else if (res == 2)
        return 2;
    return 0;
}

void Server::handleNewConnection(int epoll_fd, int sockfd) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept client connection: " << strerror(errno) << std::endl;
        return;
    }

    // char buffer[1024];
    // int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    // if (bytesRead < 0) {
    //     std::cerr << "Failed to receive data from client: " << strerror(errno) << std::endl;
    //     close(clientSocket);
    //     return;
    // }
    // buffer[bytesRead] = '\0';

    // std::cout << "Received data 1: " << buffer << std::endl;

    _tmpClients[clientSocket] = new struct clientInfo;
    _tmpClients[clientSocket]->socket_fd = clientSocket;
    _tmpClients[clientSocket]->numOfChannels = 0;
    // int res = check_message(clientInfo, buffer);
    /* if (res == 1) {
        std::cout << "Wrong Password!" << std::endl;
        // close(clientSocket);
        return;
    } else if (res == 2) {
        // close(clientSocket);
        return;
    }
    std::cout << "Nick: " << clientInfo.nick << std::endl;
    std::cout << "Pass: " << clientInfo.pass << std::endl;
    std::cout << "User: " << clientInfo.user << std::endl;*/
    // clientInfo.push_back(client_info);

    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = clientSocket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
        std::cerr << "Failed to add client socket to epoll: " << strerror(errno) << std::endl;
        close(clientSocket);
        return;
    }
}

// getArgsPro
void getArgsPro(std::vector<std::string>* args, std::pair<std::vector<std::string>, std::string> input, size_t start)
{
    size_t i;
    for (i = start; i < input.first.size(); i++)
        args->push_back(input.first[i]);
    if (!input.second.empty())
        args->push_back(input.second);
}
// getArgsPro

void Server::handleClientData(int epoll_fd, int clientSocket) {
    char buffer[1024];
    (void)epoll_fd;
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0); // Ensure space for null-terminator
    // if (bytesRead <= 0) {
    //     if (bytesRead == 0) {
    //         std::cout << "Connection closed by client" << std::endl;
    //     } else {
    //         std::cerr << "Error receiving data from client" << std::endl;
    //     }
    //     epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientSocket, NULL);
    //     close(clientSocket);
    // } else {

    buffer[bytesRead] = '\0'; // Null-terminate the received data
    _messages[clientSocket] += buffer;
    while (_messages[clientSocket].find("\n") != std::string::npos)
    {
        std::string line = _messages[clientSocket].substr(0, _messages[clientSocket].find("\n"));
        _messages[clientSocket] = _messages[clientSocket].substr(_messages[clientSocket].find("\n") + 1);
        if (line.find("\r") != std::string::npos)
            line.erase(line.find("\r"));
        std::pair<std::vector<std::string>, std::string> input;
        int res = check_message(_tmpClients[clientSocket], line);
        initInput(&input, line);
        ft_printinput(input);
        if (res == 1) {
            // close(clientSocket);
            return;
        } else if (res == 2) {
            // close(clientSocket);
            return;
        }
        std::vector<std::string> tokens = split(line, ' ');
        // Inside handleClientData function
        if (!tokens.empty()) {
            if (checkForOperators(line, _tmpClients[clientSocket]) == true) {
                std::cout << "comando executado\n";
            } else if (tokens[0] == "PRIVMSG" && tokens.size() > 1) {
                sendChannelMessage(input, _tmpClients[clientSocket]);
            }
        }
    }
    // std::cout << "Nick: " << _tmpClients[clientSocket]->nick << std::endl;
    // std::cout << "Pass: " << _tmpClients[clientSocket]->pass << std::endl;
    // std::cout << "User: " << _tmpClients[clientSocket]->user << std::endl;

    // std::cout << "recieved buffer: " << buffer << std::endl;

    // Process commands
    // }
}

int Server::epollFunction() {
    int sockfd = createUser();
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(getPortAsUint16());
    server_addr.sin_addr.s_addr = INADDR_ANY;

    int opt = 1;
	if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) != 0)
        return 1;
    if (bindUser(sockfd, server_addr))
        return 1;
    if (listenUser(sockfd))
        return 1;

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Epoll creation failed" << std::endl;
        return 1;
    }
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = sockfd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
        std::cerr << "Failed to add server socket to epoll" << std::endl;
        return 1;
    }

    std::cout << "Server started. Listening on port " << getPort() << "..." << std::endl;

    epoll_event events[MAX_EVENTS];
	//sendMessage(sockfd, "CHANLIMIT=#&:25\r\n");
    while (true) {
        int numEvents = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == sockfd) {
                handleNewConnection(epoll_fd, sockfd);
            } else {
                handleClientData(epoll_fd, events[i].data.fd);
            }
        }
    }

    close(sockfd);
    return 0;
}

std::vector<std::string> Server::splitspace(const std::string &str) {
    // std::cout << std::endl << "split" << std::endl << std::endl;
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (!ss.eof()) {
        // std::cout << "token->$" << token << "$" << std::endl;
        ss >> token;
        tokens.push_back(token);
    }
    // std::cout << std::endl << std::endl;
    return tokens;
}

std::vector<std::string> Server::split(const std::string &str, char delimiter) {
    // std::cout << std::endl << "split" << std::endl << std::endl;
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        // std::cout << "token->$" << token << "$" << std::endl;
        tokens.push_back(token);
    }
    // std::cout << std::endl << std::endl;
    return tokens;
}

void	Server::sendMessage(int fd, std::string message) {
	const char	*buffer = message.c_str();
	send(fd, buffer, std::strlen(buffer), MSG_DONTWAIT);
}

void Server::sendChannelMessage(std::pair<std::vector<std::string>, std::string> input, clientInfo* user) {
    // Find the channel

    std::vector<std::string> args;
    getArgsPro(&args, input, 1);
    ft_printinput(input);
	std::map<std::string, Channel*>::iterator it = _channelsList.find(args[0]);
	if (it == _channelsList.end()) {
		std::cerr << "Channel " << args[0] << " does not exist." << std::endl;
		return;
	}

	std::vector<int> membersFd = it->second->getMembersFd();
	if (membersFd.size() < 2) {
		std::cerr << "Channel " << args[0] << " has fewer than 2 members." << std::endl;
		return;
	}
	for (std::vector<int>::iterator itt = membersFd.begin(); itt != membersFd.end(); ++itt) {
		if (user->socket_fd != *itt) {  // Ensure not to send the message to the sender
			sendMessage(*itt, PRIVMSG(user->nick, args[0], args[1]));  // Send the message to the member
		}
	}
}
