/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:20 by marvin            #+#    #+#             */
/*   Updated: 2024/06/21 14:01:38 by joaoped2         ###   ########.fr       */
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

std::string setNonBlocking(int socket) {
    int flags = fcntl(socket, F_GETFL, 0);
    char buffer[1024];
    recv(socket, buffer, sizeof(buffer) - 1, 0);
    std::string tmp = buffer;
    if (flags == -1) {
        std::cerr << "Failed to get socket flags" << std::endl;
        return(tmp);
    }

    if (fcntl(socket, F_SETFL, flags | O_NONBLOCK) == -1) {
        std::cerr << "Failed to set non-blocking mode" << std::endl;
    }
    // std::cout << buffer << std::endl << std::endl << std::endl;
    return (tmp);
}


void Server::handleNewConnection(int epoll_fd, int sockfd) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept client connection" << std::endl;
        return;
    }

    std::string tmp = setNonBlocking(clientSocket);
    char *buffer = new char[tmp.size() + 1];
    std::strcpy(buffer, tmp.c_str());

    clientInfo clientInfo;
    clientInfo.socket_fd = clientSocket;

    // Process client message to extract nick
    int res = check_message(clientInfo, buffer); // Make sure this sets clientInfo.nick correctly
    if (res == 1) {
        std::cerr << "Failed to parse client message" << std::endl;
        close(clientSocket);
        delete[] buffer;
        return;
    }

    _clientInfo.push_back(clientInfo);
    delete[] buffer;

    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

    struct epoll_event event;
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = clientSocket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
        std::cerr << "Failed to add client socket to epoll" << std::endl;
        close(clientSocket);
        return;
    }
}



void Server::handleClientData(int epoll_fd, int clientSocket) {
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    std::cout << buffer << std::endl;
    if (bytesRead <= 0) {
        if (bytesRead == 0) {
            std::cout << "Connection closed by client" << std::endl;
        } else {
            std::cerr << "Error receiving data from client" << std::endl;
        }
        epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientSocket, NULL);
        close(clientSocket);
    } else {
        buffer[bytesRead] = '\0';
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
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

int Server::checkSingle(clientInfo& clientInfo, const std::string& result) {
    std::vector<std::string> line = split(result, ' ');
    std::string tmp, tmp2;

	for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); ++it) {
		if (*it == "PASS" && ++it != line.end()) {
			tmp = *it;
			tmp2 = getPassword();
			if (tmp.length() - 1 != tmp2.length())
				return 1;
			clientInfo.pass = *it;
		} else if (*it == "NICK" && ++it != line.end()) {
			clientInfo.nick = *it;
		} else if (*it == "USER" && ++it != line.end()) {
			clientInfo.user = *it;
		}
	}
	/*while (clientInfo.pass.empty() && clientInfo.nick.empty() && clientInfo.user.empty())
	{
		for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); ++it) {
			if (*it == "PASS" && ++it != line.end()) {
				tmp = *it;
				tmp2 = getPassword();
				if (tmp.length() - 1 != tmp2.length())
					return 1;
				clientInfo.pass = *it;
			} else if (*it == "NICK" && ++it != line.end()) {
				clientInfo.nick = *it;
			} else if (*it == "USER" && ++it != line.end()) {
				clientInfo.user = *it;
			}
		}
	}*/
    return 0;
}

int Server::check_message(clientInfo& client_info, char* buffer) {
    std::string message(buffer); // Convert char* buffer to std::string
    std::vector<std::string> tokens = split(message, '\n');

    // Process each token in the message
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        std::string result = *it;

        // Process each part of the token
        std::vector<std::string> parts = split(result, ' ');
        for (std::vector<std::string>::iterator it2 = parts.begin(); it2 != parts.end(); ++it2) {
            if (*it2 == "NICK" && ++it2 != parts.end()) {
                client_info.nick = *it2; // Set the nick from client message
                return 0;
            }
        }
    }
    return 1;
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
