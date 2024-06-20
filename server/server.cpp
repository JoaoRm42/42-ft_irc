/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:20 by marvin            #+#    #+#             */
/*   Updated: 2024/06/19 16:39:59 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() : _port("6667"), _password("password"), _client_info() {}
Server::Server(char **av) : _client_info() {
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

Server::~Server() {}

std::string Server::get_password() { return ( this->_password ); }
std::string Server::get_port() { return ( this->_port ); }

uint16_t Server::get_port_as_uint16() {
    // Convert string port to uint16_t
    char* endptr;
    unsigned long port_ul = strtoul(this->_port.c_str(), &endptr, 10);
    if (*endptr != '\0') {
        // Error handling: Invalid port number string
        std::cerr << "Invalid port number format" << std::endl;
        // Return default port or handle error appropriately
        return 0; // Example default port
    }
    return static_cast<uint16_t>(port_ul);
}

int Server::create_user() {
    return socket(AF_INET, SOCK_STREAM, 0);
}

int Server::bind_user(int sockfd, const struct sockaddr_in& server_addr) {
    if (bind(sockfd, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
        std::cerr << "Failed to bind socket" << std::endl;
        return (1);
    }
    return (0);
}

int Server::listen_user(int sockfd) {
    if (listen(sockfd, SOMAXCONN) < 0) {
        std::cerr << "Failed to listen on socket" << std::endl;
        return (1);
    }
    return (0);
}

std::vector<std::string> split(const std::string& s, char delimiter) {
    std::vector<std::string> tokens;
    std::string token;
    std::istringstream tokenStream(s);

    while (std::getline(tokenStream, token, delimiter)) {
        tokens.push_back(token);
    }

    return tokens;
}

int Server::check_single(Client_info& client_info, const std::string& result) {
    std::vector<std::string> line = split(result, ' ');
    std::string tmp, tmp2;
    for (std::vector<std::string>::iterator it = line.begin(); it != line.end(); ++it) {
        if (*it == "PASS" && ++it != line.end()) {
            tmp = *it;
            tmp2 = get_password();
            if (tmp.length() - 1 != tmp2.length())
                return (1);
            client_info.pass = *it;
            // std::cout << "Stored Pass: " << client_info.pass << std::endl;
        } else if (*it == "NICK" && ++it != line.end()) {
            client_info.nick = *it;
            // std::cout << "Stored Nick: " << client_info.nick << std::endl;
        } else if (*it == "USER" && ++it != line.end()) {
            client_info.user = *it;
            // std::cout << "Stored User: " << client_info.user << std::endl;
        } else if (*it == "QUIT")
            return (2);
    }
    return (0);
}

int Server::check_message(Client_info& client_info, char* buffer) {
    std::vector<std::string> tokens = split(buffer, '\n');
    int res;
    for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); ++it) {
        std::string result = *it;
        res = check_single(client_info, result);
        if (res == 1)
            return (1);
        else if (res == 2)
            return (2);
    }
    return (0);
}

void Server::handle_new_connection(int epoll_fd, int sockfd) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == -1) {
        std::cerr << "Failed to accept client connection" << std::endl;
        return;
    }

    char buffer[1024];
    recv(clientSocket, buffer, sizeof(buffer), 0);

    Client_info client_info;
    client_info.socket_fd = clientSocket;
    int res;
    res = check_message(client_info, buffer) == 1;
    if (res == 1) {
        std::cout << "Wrong Password!" << std::endl;
        return ;
    }
    else if (res == 2)
        return ;
    _client_info.push_back(client_info);

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

void Server::handle_client_data(int epoll_fd, int clientSocket) {
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
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

        // Find the client_info associated with this socket
        for (std::vector<Client_info>::iterator it = _client_info.begin(); it != _client_info.end(); ++it) {
            if (it->socket_fd == clientSocket) {
                std::stringstream ss;
                ss << it->socket_fd << ": " << std::string(buffer);
                std::string message = ss.str();


                //^^ Check if you can pass the nick name ^^//
                for (std::vector<Client_info>::iterator cit = _client_info.begin(); cit != _client_info.end(); ++cit) {
                    if (cit->socket_fd != clientSocket) {
                        send(cit->socket_fd, message.c_str(), message.size(), 0);
                    }
                }
                write(1, message.c_str(), message.size());
                break;
            }
        }
    }
}

int Server::epollFunction() {
    int sockfd = create_user();
    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(get_port_as_uint16());
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind_user(sockfd, server_addr))
        return (1);
    if (listen_user(sockfd))
        return (1);

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

    std::cout << "Server started. Listening on port " << get_port() << "..." << std::endl;

    epoll_event events[MAX_EVENTS];

    while (true) {
        int numEvents = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == sockfd) {
                handle_new_connection(epoll_fd, sockfd);
            } else {
                handle_client_data(epoll_fd, events[i].data.fd);
            }
        }
    }

    close(sockfd);
    return 0;
}
