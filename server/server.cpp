/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:20 by marvin            #+#    #+#             */
/*   Updated: 2024/06/18 17:38:20 by joaoped2         ###   ########.fr       */
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

int Server::listen_user(int serverSocket) {
    if (listen(serverSocket, SOMAXCONN) < 0) {
        std::cout << "Listen Failed" << std::endl;
        return (1);
    }
    return (0);
}

struct sockaddr_in Server::serveraddr() {
    struct sockaddr_in server_addr;

    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(get_port_as_uint16());

    return server_addr;
}

int Server::create_epoll() {
    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        std::cerr << "Epoll creation failed" << std::endl;
        return (1);
    }
    return (epoll_fd);
}

int add_to_server_socket(int sockfd, int epoll_fd, struct epoll_event event) {

    event.events = EPOLLIN | EPOLLOUT;
    event.data.fd = sockfd;

    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, sockfd, &event) == -1) {
        std::cerr << "Failed to add server socket to epoll" << std::endl;
        return (1);
    }
    return (0);
}

void client_connection(int sockfd, int epoll_fd, struct epoll_event event, std::vector<int> clientSockets) {
    struct sockaddr_in clientAddr;
    socklen_t clientAddrLen = sizeof(clientAddr);
    int clientSocket = accept(sockfd, (struct sockaddr*)&clientAddr, &clientAddrLen);
    if (clientSocket == -1)
        std::cerr << "Failed to accept client connection" << std::endl;
    std::cout << "New connection from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;

    // Add client socket to epoll
    event.events = EPOLLIN | EPOLLET;
    event.data.fd = clientSocket;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, clientSocket, &event) == -1) {
        std::cerr << "Failed to add client socket to epoll" << std::endl;
        close(clientSocket);
    }

    // Store client socket for later use or tracking
    clientSockets.push_back(clientSocket);
}

void client_data(struct epoll_event events[MAX_EVENTS], int i, int epoll_fd, struct epoll_event event, std::vector<int> clientSockets) {
    // Handle data from client sockets
    int clientSocket = events[i].data.fd;
    char buffer[1024];
    int bytesRead = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (bytesRead <= 0) {
        if (bytesRead == 0)
            std::cout << "Connection closed by client" << std::endl;
        else
            std::cerr << "Error receiving data from client" << std::endl;
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

int Server::epollFunction() {
    int  sockfd, epoll_fd;
    struct sockaddr_in server_addr;
    struct epoll_event event;
    struct epoll_event events[MAX_EVENTS];
    std::vector<int> clientSockets;

    epoll_fd = create_epoll();
    server_addr = serveraddr();
    sockfd = create_user();
    if (bind_user(sockfd, server_addr))
        return (1);
    if (listen_user(sockfd))
        return (1);
    if (epoll_fd == 1)
        return (1);
    if (add_to_server_socket(sockfd, epoll_fd, event))
        return (1);

    while (true) {
        int numEvents = epoll_wait(epoll_fd, events, MAX_EVENTS, -1);
        for (int i = 0; i < numEvents; ++i) {
            if (events[i].data.fd == sockfd)
                client_connection(sockfd, epoll_fd, event, clientSockets);
            else
                client_data(events, i, epoll_fd, event, clientSockets);
        }
    }
    close(sockfd);
    return (0);
}
