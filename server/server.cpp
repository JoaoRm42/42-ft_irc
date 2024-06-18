/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:20 by marvin            #+#    #+#             */
/*   Updated: 2024/06/18 15:16:54 by joaoped2         ###   ########.fr       */
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
    int serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (serverSocket == -1) {
        std::cout << "Socket Creation Failed" << std::endl;
        return (1);
    }
    return (serverSocket);
}

int Server::bind_user(int serverSocket, const struct sockaddr_in& serverAddr) {
    if (bind(serverSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cout << "Bind Failed" << std::endl;
        return (1);
    }
    return (0);
}

int listen_user(int serverSocket) {
    if (listen(serverSocket, SOMAXCONN) < 0) {
        std::cout << "Listen Failed" << std::endl;
        return (1);
    }
    return (0);
}

int Server::epollFunction() {
    int  sockfd, epoll_fd;
    struct sockaddr_in server_addr;

    sockfd = create_user();

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(get_port_as_uint16());

    bind_user(sockfd, server_addr);
    listen_user(sockfd);

    epoll_fd = epoll_create1(0);
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

    struct epoll_event events[MAX_EVENTS];
    std::vector<int> clientSockets;

    while (true) {
        int numEvents = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == sockfd) {
                struct sockaddr_in clientAddr;
                socklen_t clientAddrLen = sizeof(clientAddr);
                int clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
                if (clientSocket == -1) {
                    std::cerr << "Failed to accept client connection" << std::endl;
                    continue;
                }
                std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

                // Add client socket to epoll
                event.events = EPOLLIN | EPOLLET;
                event.data.fd = clientSocket;
                if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
                    std::cerr << "Failed to add client socket to epoll" << std::endl;
                    close(clientSocket);
                    continue;
                }

                // Store client socket for later use or tracking
                clientSockets.push_back(clientSocket);
            } else {
                // Handle data from client sockets
                int clientSocket = events[i].data.fd;
                char buffer[1024];
                int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
                if (bytesRead <= 0) {
                    // Client closed connection or error
                    if (bytesRead == 0) {
                        std::cout << "Connection closed by client" << std::endl;
                    } else {
                        std::cerr << "Error receiving data from client" << std::endl;
                    }
                    epoll_ctl(epoll_fd, EPOLL_CTL_DEL, clientSocket, &event);
                    close(clientSocket);
                } else {
                    for (std::vector<int>::iterator it = clientSockets.begin(); it != clientSockets.end(); ++it) {
                        if (*it != clientSocket)
                            send(*it, buffer, bytesRead, 0);
                    }
                    write(1, &buffer, bytesRead);
                }
            }
        }
    }

    close(sockfd);
    return 0;
}
