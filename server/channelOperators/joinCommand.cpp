/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   joinCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macastan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:37:20 by macastan          #+#    #+#             */
/*   Updated: 2024/07/03 16:37:22 by macastan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../../libs.hpp"
void Server::tryToJoinChannel(std::string& channelName, Client *user, std::vector<std::string> tokens) {
	// Check if the channel exists, create if it does not
	if (user->getNumOfChannels() >= LIMITOFCHANNELS)
	{
		std::string msgTooManyChannels = ":" + displayHostname() + " 405 " + user->getNick() + " " + channelName + " :You have joined too many channels\r\n";
		sendMessage(user->getSocketFD(), msgTooManyChannels);
		return ;
	}
	for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); it++)
	{
		if (it->first == channelName)
		{
			if (tokens.size() > 2)
				joinExistingChannel(channelName, it->second, user, tokens[2], 1);
			else
			{
				std::string	noPass = "no password";
				joinExistingChannel(channelName, it->second, user, noPass, 0);
			}
			return ;
		}
	}

	Channel* newChannel = new Channel(channelName);
	_channelsList[channelName] = newChannel;
	user->addBackChannel(channelName);
	newChannel->setListOfMembers(user);
	newChannel->setListOfAdmins(user);

	// Add the Bot
	BotJoinChannel(channelName);

	std::string	msgJoin = ":" + user->getNick() + " JOIN " + channelName + "\r\n";
	sendMessage(user->getSocketFD(), msgJoin);

	std::string msgMode = ":" + user->getNick() + " MODE " + channelName + " +t\r\n";
	newChannel->setTopicOn(true);
	sendMessage(user->getSocketFD(), msgMode);

	std::string msgNamReply = ":" + displayHostname() + " 353 " + user->getNick() + " = " + channelName + " :@" + user->getNick() + "\r\n";
	sendMessage(user->getSocketFD(), msgNamReply);

	std::string msgEndOfList = ":" + displayHostname() + " 366 " + user->getNick() + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user->getSocketFD(), msgEndOfList);

	//Send the user a priv message from BOT to explain the command that bot do
    sendHelpTableBot(user);
}

void	Server::joinExistingChannel(std::string channelName, Channel *thisChannel, Client *user, std::string channelPass, int flag) {
	//if the user is already on the channel nothing is done, and return
	for (size_t k = 0; k != thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == user->getNick())
			return;
	}
	//check if the channel is invite only, and don´t let the user enter if it is invite only
	if (thisChannel->getInviteOnly())
	{
		std::string msgInviteOnly = ":" + displayHostname() + " 473 " + user->getNick() + " " + channelName + " :Cannot join channel (+i)\r\n";
		sendMessage(user->getSocketFD(), msgInviteOnly);
		return;
	}
	//check if the channel needs a password to enter and also check if the password giving is right
	if (thisChannel->getPasswordNeed())
	{
		if (flag == 0 || thisChannel->getPassword() != channelPass)
		{
			std::string msgWrongPassword = ":" + displayHostname() + " 475 " + user->getNick() + " " + channelName + " :Cannot join channel (+k)\r\n";
			sendMessage(user->getSocketFD(), msgWrongPassword);
			return;
		}
	}
	//check if the channel is already full, with the num max of users
	if (thisChannel->getNumOfMembers() >= thisChannel->getNumMaxOfMembers())
	{
		std::string msgChannelFull = ":" + displayHostname() + " 471 " + user->getNick() + " " + channelName + " :Cannot join channel (+l)\r\n";
		sendMessage(user->getSocketFD(), msgChannelFull);
		return;
	}
	//add the bot if it isn't on the channel
	size_t k;
	for (k = 0; k != thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == "BOT")
			break;
	}
	if (k == thisChannel->getlistOfMembers().size())
		BotJoinChannel(channelName);
	//put the user on that channel
	user->addBackChannel(channelName);
	thisChannel->setListOfMembers(user);
	std::string allMembers = thisChannel->getMembersForList();

	std::string	msgJoin = ":" + user->getNick() + " JOIN " + channelName + "\r\n";
	sendMessage(user->getSocketFD(), msgJoin);

	std::string msgNamReply = ":" + displayHostname() + " 353 " + user->getNick() + " = " + channelName + " :" + allMembers + "\r\n";
	sendMessage(user->getSocketFD(), msgNamReply);

	std::string msgEndOfList = ":" + displayHostname() + " 366 " + user->getNick() + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user->getSocketFD(), msgEndOfList);

	//Send the user a priv message from BOT to explain the command that bot do
	if (user->getNick() != "BOT")
  	  sendHelpTableBot(user);

	//send a messagem to all the members of the channel saying that someone joined the channel
	std::string msgJoinBroadcast = ":" + user->getNick() + " JOIN :" + channelName + "\r\n";
	for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
	{
		if (thisChannel->getMembersFd()[i] != user->getSocketFD())
			sendMessage(thisChannel->getMembersFd()[i], msgJoinBroadcast);
	}
}
