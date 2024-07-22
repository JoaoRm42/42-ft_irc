/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   modeCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macastan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:38:06 by macastan          #+#    #+#             */
/*   Updated: 2024/07/03 16:38:10 by macastan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../../libs.hpp"

void	Server::showMode(std::string& channelName, Client *user) {
	std::map<std::string, Channel *>::iterator it;
	Channel *thisChannel;
	//check if the channel exists
	for (it = _channelsList.begin(); it != _channelsList.end(); it++)
	{
		if (it->first == channelName)
		{
			thisChannel = it->second;
			break;
		}
	}
	if (it == _channelsList.end())
	{
		std::string msgNotOnChannel = ":" + displayHostname() + " 403 " + user->getNick() + " " + channelName + " :No such channel\r\n";
		sendMessage(user->getSocketFD(), msgNotOnChannel);
		return;
	}
	//send the /mode message with the set modes and the args (the limit and the key/password)
	std::string allModes = thisChannel->getAllModes();
	std::string	msgWhatMode = ":" + displayHostname() + " 324 " + user->getNick() + " " + channelName + " " + allModes + "\r\n";
	sendMessage(user->getSocketFD(), msgWhatMode);
	if (user->getIsHexchat() == false)
	{
		char* dt = std::ctime(&thisChannel->getCreationTime());
		std::string dateString(dt);
		size_t pos = dateString.find('\n');
		if (pos != std::string::npos)
			dateString.erase(pos);
		std::string	msgCreationTime = ":" + displayHostname() + " 329 " + user->getNick() + " " + channelName + " " + dateString + "\r\n";
		sendMessage(user->getSocketFD(), msgCreationTime);
	}
	else
	{
		std::string	msgCreationTime = ":" + displayHostname() + " 329 " + user->getNick() + " " + channelName + " " + thisChannel->getCreationTimeString() + "\r\n";
		sendMessage(user->getSocketFD(), msgCreationTime);
	}
}

void	Server::tryToMode(std::string& channelName, Client *user, std::vector<std::string> tokens){
	std::map<std::string, Channel *>::iterator it;
	Channel *thisChannel;
	//check if the channel exists
	for (it = _channelsList.begin(); it != _channelsList.end(); it++)
	{
		if (it->first == channelName)
		{
			thisChannel = it->second;
			break;
		}
	}
	if (it == _channelsList.end())
	{
		std::string msgNotOnChannel = ":" + displayHostname() + " 403 " + user->getNick() + " " + channelName + " :No such channel\r\n";
		sendMessage(user->getSocketFD(), msgNotOnChannel);
		return;
	}
	size_t k;
	//check the user is on that channel
	for (k = 0; k < thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == user->getNick())
			break ;
	}
	if (k == thisChannel->getlistOfMembers().size())
	{
		std::string msgNotOnChannel = ":" + displayHostname() + " 442 " + user->getNick() + " " + channelName + " :You're not on that channel\r\n";
		sendMessage(user->getSocketFD(), msgNotOnChannel);
		return;
	}
	//check the user is an operator on that channel
	for (k = 0; k < thisChannel->getlistOfAdmins().size(); k++)
	{
		if (thisChannel->getlistOfAdmins()[k] == user->getNick())
			break ;
	}
	if (k == thisChannel->getlistOfAdmins().size())
	{
		std::string msgNotAnOp = ":" + displayHostname() + " 482 " + user->getNick() + " " + channelName + " :You're not channel operator\r\n";
		sendMessage(user->getSocketFD(), msgNotAnOp);
		return;
	}
	//do channel modes if it has enough parameters
	if (tokens.size() >= 3 && (tokens[2][0] == '+' || tokens[2][0] == '-'))
		modeChannel(user, tokens, thisChannel);
}

void	Server::modeUserOperator(Client *user, std::string userToGetOp, Channel *thisChannel, int flag) {
	size_t	k = 0;
	//check if the nick is in the server to give operator
	for (k = 0; k < thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == userToGetOp)
			break ;
	}
	if (k == thisChannel->getlistOfMembers().size())
	{
		std::string msgNoSuchNick = ":" + displayHostname() + " 401 " + user->getNick() + " " + thisChannel->getChannelName() + " " + userToGetOp + " :No such nick\r\n";
		sendMessage(user->getSocketFD(), msgNoSuchNick);
		return;
	}
	//give the operator privilege if the user is not an operator already
	if (flag == 1 && !thisChannel->isAdm(userToGetOp))
	{
		thisChannel->setListOfAdmins(_getUserClass(userToGetOp));
		std::string msgModeGiveOperator = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " +o " + userToGetOp + "\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeGiveOperator);
	}
	//take the operator privilege if the user is an operator
	else if (flag == -1 && thisChannel->isAdm(userToGetOp))
	{
		thisChannel->removeAdmin(_getUserClass(userToGetOp));
		std::string msgModeRemoveOperator = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " -o " + userToGetOp + "\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeRemoveOperator);
	}
}

void	Server::modeChannel(Client *user, std::vector<std::string> tokens, Channel *thisChannel) {
	int	flag = 0; //if =-1 (minus mode) else if =+1 (plus mode)
	//see all the string because you can send plus than one mode
	size_t j = 0;
	for (size_t i = 0; i < tokens[2].size(); i++)
	{
		if (tokens[2][i] == '+')
			flag = 1;
		else if (tokens[2][i] == '-')
			flag = -1;
		else if (tokens[2][i] == 'i')
			inviteMode(flag, thisChannel, user);
		else if (tokens[2][i] == 't')
			topicMode(flag, thisChannel, user);
		else if (tokens[2][i] == 'k')
		{
			if ((3 + j) < tokens.size())
			{
				keyMode(flag, thisChannel, user, tokens[3 + j]);
				j++;
			}
			else
			{
				std::string msgModeSpecifyKey = ":" + displayHostname() + " 324 " + user->getNick() + " " + thisChannel->getChannelName() + " :You must specify a parameter for the key mode\r\n";
				sendMessage(user->getSocketFD(), msgModeSpecifyKey);
			}
		}
		else if (tokens[2][i] == 'l')
		{
			if (flag == 1 && (3 + j) < tokens.size())
			{
				limitUserMode(flag, thisChannel, user, tokens[3 + j]);
				j++;
			}
			else if (flag == -1)
				limitUserMode(flag, thisChannel, user, "unset the limit");
			else
			{
				std::string msgModeSpecifyLimit = ":" + displayHostname() + " 324 " + user->getNick() + " " + thisChannel->getChannelName() + " :You must specify a parameter for the limit mode\r\n";
				sendMessage(user->getSocketFD(), msgModeSpecifyLimit);
			}
		}
		else if (tokens[2][i] == 'o')
		{
			if (flag == 1 && (3 + j) < tokens.size())
			{
				modeUserOperator(user, tokens[3 + j], thisChannel, flag);
				j++;
			}
			else if (flag == -1 && (3 + j) < tokens.size())
				modeUserOperator(user, tokens[3 + j], thisChannel, flag);
			else
			{
				std::string msgModeSpecifyLimit = ":" + displayHostname() + " 324 " + user->getNick() + " " + thisChannel->getChannelName() + " :You must specify a parameter for the operator mode\r\n";
				sendMessage(user->getSocketFD(), msgModeSpecifyLimit);
			}
		}
		else
		{
			std::string msgModeUnknownFlag = ":" + displayHostname() + " 501 " + user->getNick() + " :Unknown MODE flag\r\n";
			sendMessage(user->getSocketFD(), msgModeUnknownFlag);
		}
	}
}

void	Server::inviteMode(int flag, Channel *thisChannel, Client *user) {
	if (flag == 1 && thisChannel->getInviteOnly() == false)
	{
		thisChannel->setInviteOnly(true);
		std::string msgModeInviteOnly = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " +i\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeInviteOnly);
	}
	else if (flag == -1 && thisChannel->getInviteOnly() == true)
	{
		thisChannel->setInviteOnly(false);
		std::string msgModeNotInviteOnly = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " -i\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeNotInviteOnly);
	}
}

void	Server::topicMode(int flag, Channel *thisChannel, Client *user) {
	if (flag == 1 && thisChannel->getTopicOn() == false)
	{
		thisChannel->setTopicOn(true);
		std::string msgModeTopicOn = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " +t\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeTopicOn);
	}
	else if (flag == -1 && thisChannel->getTopicOn() == true)
	{
		thisChannel->setTopicOn(false);
		std::string msgModeTopicOff = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " -t\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeTopicOff);
	}
}

void	Server::keyMode(int flag, Channel *thisChannel, Client *user, std::string key) {
	if (flag == 1 && thisChannel->getPasswordNeed() == false)
	{
		thisChannel->setKeyPass(key, true);
		std::string msgModeKey = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " +k" + " " + key + "\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeKey);
	}
	else if (flag == -1 && thisChannel->getPasswordNeed() == true)
	{
		if (thisChannel->getPassword() == key)
		{
			thisChannel->setKeyPass(key, false);
			std::string msgModeNotKey = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " -k\r\n";
			for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
				sendMessage(thisChannel->getMembersFd()[i], msgModeNotKey);
		}
		else
		{
			std::string msgModeUnknownKey = ":" + displayHostname() + " 525 " + user->getNick() + " :Key is not well-formed\r\n";
			sendMessage(user->getSocketFD(), msgModeUnknownKey);
		}
	}
}

void	Server::limitUserMode(int flag, Channel *thisChannel, Client *user, std::string limit) {
	size_t	newLimit = std::atoi(limit.c_str());
	for (size_t i = 0; i < limit.size(); i++)
	{
		if (!isdigit(limit[i]))
			newLimit = 0;
	}
	if (flag == -1 && thisChannel->getLimitOfUsers() == true)
	{
		thisChannel->setLimitOfUsers(std::numeric_limits<int>::max(), false);
		std::string msgModeNoLimit = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " -l\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeNoLimit);
	}
	else if (flag == 1 && thisChannel->getNumMaxOfMembers() != newLimit)
	{
		thisChannel->setLimitOfUsers(newLimit, true);
		std::string msgModeNoLimit = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " +l" + " " + limit + "\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeNoLimit);
	}
}