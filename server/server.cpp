/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:20 by marvin            #+#    #+#             */
/*   Updated: 2024/06/27 14:30:01 by joaoped2         ###   ########.fr       */
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

int Server::checkSingle(clientInfo& clientInfo, char* result) {
    std::vector<std::string> line = split(result, '\n');
    std::string clientPassword;
    std::string serverPassword = getPassword();
    std::string content(result);

    while (1) {
        for (std::vector<std::string>::iterator lineIt = line.begin(); lineIt != line.end(); ++lineIt) {
            std::string saver = *lineIt;
            std::vector<std::string> tokens = split(saver, ' ');
            for (std::vector<std::string>::iterator line2It = tokens.begin(); line2It != tokens.end(); ++line2It) {
                if (*line2It == "PASS") {
                    line2It++;
                    clientPassword = *line2It;
                    if (clientPassword.length() - 1 != serverPassword.length())
                    {
                        serverPassword.clear();
                        return 1;
                    }
                    clientInfo.pass = *line2It;
                    serverPassword.clear();
                } else if (*line2It == "NICK") {
                    line2It++;
                    clientInfo.nick = *line2It;
                } else if (*line2It == "USER") {
                    line2It++;
                    clientInfo.user = *line2It;
                }
            }
        }
        // std::cout << "Nick: " << clientInfo.nick << " Pass: " << clientInfo.pass << " User: " << clientInfo.user << std::endl;
        if (!clientInfo.nick.empty() && !clientInfo.pass.empty() && !clientInfo.user.empty())
            break;
    }
    return 0;
}

int Server::check_message(clientInfo& client_info, char* buffer) {
    int res = checkSingle(client_info, buffer);
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

    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    // if (bytesRead < 0) {
    //     std::cerr << "Failed to receive data from client: " << strerror(errno) << std::endl;
    //     close(clientSocket);
    //     return;
    // }
    buffer[bytesRead] = '\0';

    // std::cout << "Received data 1: " << buffer << std::endl;

    clientInfo clientInfo;
    clientInfo.socket_fd = clientSocket;
    int res = check_message(clientInfo, buffer);
    if (res == 1) {
        std::cout << "Wrong Password!" << std::endl;
        close(clientSocket);
        return;
    } else if (res == 2) {
        close(clientSocket);
        return;
    }
    std::cout << "Nick: " << clientInfo.nick << std::endl;
    std::cout << "Pass: " << clientInfo.pass << std::endl;
    std::cout << "User: " << clientInfo.user << std::endl;
    _clientInfo.push_back(clientInfo);

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

void Server::handleClientData(int epoll_fd, int clientSocket) {
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0); // Ensure space for null-terminator
    if (bytesRead <= 0) {
        if (bytesRead == 0) {
            std::cout << "Connection closed by client" << std::endl;
        } else {
            std::cerr << "Error receiving data from client" << std::endl;
        }
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientSocket, NULL);
        close(clientSocket);
    } else {
        buffer[bytesRead] = '\0'; // Null-terminate the received data

        // Find the clientInfo associated with this socket
        for (std::vector<clientInfo>::iterator it = _clientInfo.begin(); it != _clientInfo.end(); ++it) {
			if (it->socket_fd == clientSocket) {
				std::stringstream ss;
				ss << it->nick << ": " << std::string(buffer);
				std::string message = ss.str();

				// Process commands
				std::vector<std::string> tokens = split(buffer, ' ');
				// Inside handleClientData function
				if (!tokens.empty()) {
					if (checkForOperators(buffer, *it) == true) {
						std::cout << "comando executado\n";
					} else if (tokens[0] == "PRIVMSG" && tokens.size() > 1) {
						sendChannelMessage(tokens[1], buffer, *it);
						std::cout << "manda mensagem normal \n";
					}
					break;
				}
			}
		}
    }
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

void Server::sendChannelMessage(std::string channelName, std::string message, clientInfo& user) {
    // Find the channel

	if (channelName.find('\r') != std::string::npos)
		channelName.erase(channelName.find('\r'));

	std::map<std::string, Channel*>::iterator it = _channelsList.find(channelName);
	if (it == _channelsList.end()) {
		std::cerr << "Channel " << channelName << " does not exist." << std::endl;
		return;
	}

	std::vector<int> membersFd = it->second->getMenbersFd();
	if (membersFd.size() < 2) {
		std::cerr << "Channel " << channelName << " has fewer than 2 members." << std::endl;
		return;
	}
	for (std::vector<int>::iterator itt = membersFd.begin(); itt != membersFd.end(); ++itt) {
		if (user.socket_fd != *itt) {  // Ensure not to send the message to the sender
			std::string msg = ":" + user.nick + " PRIVMSG " + channelName + " :" + message + "\r\n";
			sendMessage(*itt, msg);  // Send the message to the member
		}
	}
}
