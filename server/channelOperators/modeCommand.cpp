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

	std::string	msgCreationTime = ":" + displayHostname() + " 329 " + user->getNick() + " " + channelName + " " + thisChannel->getCreationTimeString() + "\r\n";
	sendMessage(user->getSocketFD(), msgCreationTime);
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
	//divide the two types of modes, user modes or channel modes
	if (tokens.size() >= 3 && (tokens[2][0] == '+' || tokens[2][0] == '-'))
		modeChannel(user, tokens, thisChannel);
	else if (tokens.size() >= 4)
		modeUser(user, tokens, thisChannel);
}

void	Server::modeUser(Client *user, std::vector<std::string> tokens, Channel *thisChannel) {
	size_t	k = 0;
	//check if the nick is in the server to give operator
	for (k = 0; k < thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == tokens[2])
			break ;
	}
	if (k == thisChannel->getlistOfMembers().size())
	{
		std::string msgNoSuchNick = ":" + displayHostname() + " 401 " + user->getNick() + " " + thisChannel->getChannelName() + " " + tokens[2] + " :No such nick\r\n";
		sendMessage(user->getSocketFD(), msgNoSuchNick);
		return;
	}
	//give the operator privilege if the user is not an operator already
	if (tokens[3] == "+o" && !thisChannel->isAdm(tokens[2]))
	{
		thisChannel->setListOfAdmins(_getUserClass(tokens[2]));
		std::string msgModeGiveOperator = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " +o " + tokens[2] + "\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeGiveOperator);
	}
	//take the operator privilege if the user is an operator
	else if (tokens[3] == "-o" && thisChannel->isAdm(tokens[2]))
	{
		thisChannel->removeAdmin(_getUserClass(tokens[2]));
		std::string msgModeRemoveOperator = ":" + user->getNick() + " MODE " + thisChannel->getChannelName() + " -o " + tokens[2] + "\r\n";
		for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
			sendMessage(thisChannel->getMembersFd()[i], msgModeRemoveOperator);
	}
	//check if there is other flag, and don´t do it
	else if (tokens[3] != "-o" || tokens[3] != "-o")
	{
		std::string msgModeUnknownFlag = ":" + displayHostname() + " 501 " + user->getNick() + " :Unknown MODE flag\r\n";
		sendMessage(user->getSocketFD(), msgModeUnknownFlag);
	}
}

void	Server::modeChannel(Client *user, std::vector<std::string> tokens, Channel *thisChannel) {
	int	flag = 0; //if =-1 (minus mode) else if =+1 (plus mode)
	if (tokens[2][0] == '+')
		flag = 1;
	else if (tokens[2][0] == '-')
		flag = -1;
	//see all the string because you can send plus than one mode
	for (size_t i = 1; i < tokens[2].size(); i++)
	{
		size_t j = 0;
		if (tokens[2][i] == 'i')
			inviteMode(flag, thisChannel, user);
		else if (tokens[2][i] == 't')
			topicMode(flag, thisChannel, user);
		else if (tokens[2][i] == 'k')
		{
			if (tokens.size() >= (3 + j))
				keyMode(flag, thisChannel, user, tokens[3 + j++]);
			else
			{
				std::string msgModeSpecifyKey = ":" + displayHostname() + " 324 " + user->getNick() + " " + thisChannel->getChannelName() + " :You must specify a parameter for the key mode\r\n";
				sendMessage(user->getSocketFD(), msgModeSpecifyKey);
			}
		}
		else if (tokens[2][i] == 'l')
		{
			if (flag == 1 && tokens.size() >= (3 + j))
				limitUserMode(flag, thisChannel, user, tokens[3 + j++]);
			else if (flag == -1)
				limitUserMode(flag, thisChannel, user, "unset the limit");
			else
			{
				std::string msgModeSpecifyLimit = ":" + displayHostname() + " 324 " + user->getNick() + " " + thisChannel->getChannelName() + " :You must specify a parameter for the limit mode\r\n";
				sendMessage(user->getSocketFD(), msgModeSpecifyLimit);
			}
		}
		else if (tokens[2][i] != 'o')
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