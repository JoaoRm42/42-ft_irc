/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.cpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:57:40 by macastan          #+#    #+#             */
/*   Updated: 2024/07/01 19:03:22 by joaoped2         ###   ########.fr       */
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

void	Channel::setListOfMembers(clientInfo *user) {
	_listOfMembers.push_back(user->nick);
	_membersFd.push_back(user->socket_fd);
	_numOfMembers++;
}

void	Channel::setListOfAdmins(clientInfo *user) {
	_listOfAdmins.push_back(user->nick);
}

std::vector<int>	Channel::getMembersFd() {
	return (this->_membersFd);
}

std::vector<std::string>	Channel::getlistOfAdmins() {
	return (this->_listOfAdmins);
}

std::vector<std::string>	Channel::getlistOfMembers() {
	return (this->_listOfMembers);
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

/*void	Channel::sendMessageChannel(int fd, std::string message) {
	const char	*buffer = message.c_str();
	send(fd, buffer, std::strlen(buffer), MSG_DONTWAIT);
}

void Channel::joinBroadcastChannel(clientInfo *user, std::string message) {
	for (size_t i = 0; i < _membersFd.size(); i++)
	{
		if (_membersFd[i] != user->socket_fd)
			sendMessageChannel(_membersFd[i], message);
	}
}*/

std::string	Channel::getMembersForList() {
	std::string	allMembers;

	size_t tmp = 0;
	for (size_t i = 0; i < _listOfMembers.size(); i++)
	{
		bool	isAdm = false;
		for (size_t j = 0; j < _listOfAdmins.size(); j++)
		{
			if (_listOfMembers[i] == _listOfAdmins[j])
			{
				isAdm = true;
				break;
			}
		}
		if (isAdm == true)
		{
			if (tmp > 0)
				allMembers += " ";
			allMembers += "@" + _listOfAdmins[i];
			tmp++;
		}
		else
		{
			if (tmp > 0)
				allMembers += " ";
			allMembers += _listOfMembers[i];
			tmp++;
		}
	}
	return (allMembers);
}

void Channel::removeUser(clientInfo *user) {
	for (std::vector<std::string>::iterator	it = _listOfMembers.begin(); it != _listOfMembers.end(); it++)
	{
		if (*it == user->nick)
		{
			_listOfMembers.erase(it);
			break ;
		}
	}
	for (std::vector<std::string>::iterator	it = _listOfAdmins.begin(); it != _listOfAdmins.end(); it++)
	{
		if (it->c_str() == user->nick)
		{
			_listOfAdmins.erase(it);
			break ;
		}
	}
	for (std::vector<int>::iterator	it = _membersFd.begin(); it != _membersFd.end(); it++)
	{
		if (*it == user->socket_fd)
		{
			_membersFd.erase(it);
			break ;
		}
	}
	_numOfMembers--;
}

int	Channel::getOneUserFd(std::string userName) {
	int	position = 0;
	for (std::vector<std::string>::iterator	it = _listOfMembers.begin(); it != _listOfMembers.end(); it++)
	{
		if (*it == userName)
		{
			_listOfMembers.erase(it);
			break ;
		}
		position++;
	}
	return (_membersFd[position]);
}

void Channel::removeUser2(std::string userName, int fdUser) {
	for (std::vector<std::string>::iterator	it = _listOfMembers.begin(); it != _listOfMembers.end(); it++)
	{
		if (*it == userName)
		{
			_listOfMembers.erase(it);
			break ;
		}
	}
	for (std::vector<std::string>::iterator	it = _listOfAdmins.begin(); it != _listOfAdmins.end(); it++)
	{
		if (it->c_str() == userName)
		{
			_listOfAdmins.erase(it);
			break ;
		}
	}
	for (std::vector<int>::iterator	it = _membersFd.begin(); it != _membersFd.end(); it++)
	{
		if (*it == fdUser)
		{
			_membersFd.erase(it);
			break ;
		}
	}
	_numOfMembers--;
}