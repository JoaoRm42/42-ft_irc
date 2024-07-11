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
	setCrationTimeString();
}

Channel::~Channel() {}

void	Channel::removeBotFromChannel() {
	_listOfMembers.clear();
}

void	Channel::setListOfMembers(Client *user) {
	_listOfMembers.push_back(user->getNick());
	_membersFd.push_back(user->getSocketFD());
	_numOfMembers++;
}

void	Channel::setListOfAdmins(Client *user) {
	_listOfAdmins.push_back(user->getNick());
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

bool	Channel::getTopicOn() {
	return (_topicOn);
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
		if (isAdm)
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

void	Channel::removeAdmin(Client *user) {
	for (std::vector<std::string>::iterator	it = _listOfAdmins.begin(); it != _listOfAdmins.end(); it++)
	{
		if (it->c_str() == user->getNick())
		{
			_listOfAdmins.erase(it);
			break ;
		}
	}
}

bool	Channel::isAdm(std::string userName) {
	for (std::vector<std::string>::iterator	it = _listOfAdmins.begin(); it != _listOfAdmins.end(); it++)
	{
		if (it->c_str() == userName)
			return (true);
	}
	return (false);
}

void	Channel::removeUser(Client *user) {
	for (std::vector<std::string>::iterator	it = _listOfMembers.begin(); it != _listOfMembers.end(); it++)
	{
		if (*it == user->getNick())
		{
			_listOfMembers.erase(it);
			break ;
		}
	}
	for (std::vector<std::string>::iterator	it = _listOfAdmins.begin(); it != _listOfAdmins.end(); it++)
	{
		if (it->c_str() == user->getNick())
		{
			_listOfAdmins.erase(it);
			break ;
		}
	}
	for (std::vector<int>::iterator	it = _membersFd.begin(); it != _membersFd.end(); it++)
	{
		if (*it == user->getSocketFD())
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

void Channel::removeUserKick(std::string userName, int fdUser) {
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

void	Channel::setInviteOnly(bool flag) {

	if (flag == false)
		_inviteOnly = false;
	else
		_inviteOnly = true;
}

std::string	Channel::getChannelName() {
	return(_channelName);
}

void	Channel::setTopicOn(bool flag) {
	if (flag == false)
	{
		_topic.clear();
		_topicOn = false;
	}
	else
		_topicOn = true;
}

std::string	Channel::getTopic() {
	return (_topic);
}

void	Channel::setTopic(std::string newTopic, std::string user) {
	_topic = newTopic;
	_topicNick = user;
	_topicTime = time(0);
	time(&_topicTime);

	std::ostringstream out;
	out << _topicTime;
	_topicTimeString = out.str();
}

std::string Channel::getTopicNick() {
	return (_topicNick);
}

std::string Channel::getTopicTime() {
	return (_topicTimeString);
}

void	Channel::setKeyPass(std::string key, bool keyOn) {
	if (keyOn == true)
	{
		_passwordNeed = true;
		_password = key;
	}
	else
	{
		_passwordNeed = false;
		_password = "/0";
	}
}

bool	Channel::getLimitOfUsers() {
	return (_limitOfUsers);
}

void	Channel::setLimitOfUsers(size_t limit, bool limitOn) {
	if (limitOn == true)
		_limitOfUsers = true;
	else if (limitOn == false)
		_limitOfUsers = false;
	_numMaxOfMembers = limit;
}

std::string	Channel::getAllModes() {
	std::string allModes = "+";
	std::string allArgs = " ";
	if (_inviteOnly == true)
		allModes += "i";
	if (_passwordNeed == true)
	{
		allModes += "k";
		allArgs += getPassword() + " ";
	}
	if (_limitOfUsers == true)
	{
		allModes += "l";
		std::stringstream test;
		test << getNumMaxOfMembers();
		std::string x;
		test >> x;
		allArgs += x;
	}
	if (_topicOn == true)
		allModes += "t";
	return (allModes + allArgs);
}

std::time_t	Channel::getCreationTime() {
	return (_creationTime);
}

void	Channel::setCrationTimeString() {
	_creationTime = time(0);
	time(&_creationTime);

	std::ostringstream oss;
	oss << _creationTime;
	_creationTimeString = oss.str();
}

std::string	Channel::getCreationTimeString() {
	return (_creationTimeString);
}