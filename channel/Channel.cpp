/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macastan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:57:40 by macastan          #+#    #+#             */
/*   Updated: 2024/06/18 14:57:42 by macastan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Channel.hpp"

Channel::Channel(std::string name) : _channelName(name) {
	_inviteOnly = false;
	_numOfMembers = 0;
	_passwordNeed = false;
	_numMaxOfMembers = std::numeric_limits<int>::max();
}

Channel::~Channel() {}

void	Channel::setListOfMembers(clientInfo &user) {
	_listOfMembers.push_back(user.nick);
	_membersFd.push_back(user.socket_fd);
	_numOfMembers++;
}

void	Channel::setListOfAdmins(clientInfo &user) {
	_listOfAdmins.push_back(user.nick);
}

std::vector<int>	Channel::getMenbersFd() {
	return (this->_membersFd);
}

int	Channel::getFirstFd() {
	return (this->_membersFd[0]);
}

bool	Channel::getInviteOnly() {
	return (_inviteOnly);
}

bool	Channel::getPasswordNeed() {
	return (_passwordNeed);
}

std::string	Channel::getPassword(){
	return (_password);
}

size_t	Channel::getNumOfMembers() {
	return (_numOfMembers);
}

size_t	Channel::getNumMaxOfMembers() {
	return (_numMaxOfMembers);
}