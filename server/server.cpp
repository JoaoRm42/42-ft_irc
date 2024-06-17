/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:20 by marvin            #+#    #+#             */
/*   Updated: 2024/06/17 11:11:20 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "server.hpp"

Server::Server() : _port("6667"), _password("password") {}
Server::Server(char **av) {
    this->_port = av[1];
    this->_password = av[2];
    // print_private();
}

Server::Server(const Server &obj) {
    (void)obj;
}

Server& Server::operator=(const Server &obj) {
    (void)obj;
    return(*this);
}

Server::~Server() {}

void Server::print_private() {
    std::cout << this->_port << std::endl;
    std::cout << this->_password << std::endl;
}