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

void Server::tryToJoinChannel(std::string& channelName, clientInfo* user, std::vector<std::string> tokens) {
	// Check if the channel exists, create if it does not

	if (user->numOfChannels >= LIMITOFCHANNELS)
	{
		std::string msgTooManyChannels = ":" + displayHostname() + " 405 " + user->nick + " " + channelName + " :You have joined too many channels\r\n";
		sendMessage(user->socket_fd, msgTooManyChannels);
		return ;
	}
	if (channelName.find('\r') != std::string::npos)
		channelName.erase(channelName.find('\r'));
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
	//adicionar channel ao cliente e adicionar ++ ao num de canais
	user->channels.push_back(channelName);
	user->numOfChannels++;
	newChannel->setListOfMembers(user);
	newChannel->setListOfAdmins(user);

	std::string	msgJoin = ":" + user->nick + " JOIN " + channelName + "\r\n";
	sendMessage(user->socket_fd, msgJoin);

	std::string msgMode = ":" + user->nick + " MODE " + channelName + " +t\r\n";
	sendMessage(user->socket_fd, msgMode);

	std::string msgNamReply = ":" + displayHostname() + " 353 " + user->nick + " = " + channelName + " :@" + user->nick + "\r\n";
	sendMessage(user->socket_fd, msgNamReply);

	std::string msgEndOfList = ":" + displayHostname() + " 366 " + user->nick + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user->socket_fd, msgEndOfList);
}

void	Server::joinExistingChannel(std::string channelName, Channel *thisChannel, clientInfo *user, std::string channelPass, int flag) {
	for (size_t k = 0; k != thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == user->nick)
			return;
	}
	if (thisChannel->getInviteOnly() == true)
	{
		std::string msgInviteOnly = ":" + displayHostname() + " 473 " + user->nick + " " + channelName + " :Cannot join channel (+i)\r\n";
		sendMessage(user->socket_fd, msgInviteOnly);
		return;
	}
	if (thisChannel->getPasswordNeed() == true)
	{
		if (channelPass.find('\r') != std::string::npos)
			channelPass.erase(channelPass.find('\r'));
		if (flag == 0 || thisChannel->getPassword() != channelPass)
		{
			std::string msgWrongPassword = ":" + displayHostname() + " 475 " + user->nick + " " + channelName + " :Cannot join channel (+k)\r\n";
			sendMessage(user->socket_fd, msgWrongPassword);
			return;
		}
	}
	if (thisChannel->getNumOfMembers() >= thisChannel->getNumMaxOfMembers())
	{
		std::string msgChannelFull = ":" + displayHostname() + " 471 " + user->nick + " " + channelName + " :Cannot join channel (+l)\r\n";
		sendMessage(user->socket_fd, msgChannelFull);
		return;
	}

	user->channels.push_back(channelName);
	thisChannel->setListOfMembers(user);
	std::string allMembers = thisChannel->getMembersForList();

	std::string	msgJoin = ":" + user->nick + " JOIN " + channelName + "\r\n";
	sendMessage(user->socket_fd, msgJoin);

	//verificar necessidade de setar o mode a +t
	std::string msgMode = ":" + user->nick + " MODE " + channelName + " +t\r\n";
	sendMessage(user->socket_fd, msgMode);

	std::string msgNamReply = ":" + displayHostname() + " 353 " + user->nick + " = " + channelName + " :" + allMembers + "\r\n";
	sendMessage(user->socket_fd, msgNamReply);

	std::string msgEndOfList = ":" + displayHostname() + " 366 " + user->nick + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user->socket_fd, msgEndOfList);

	std::string msgJoinBroadcast = ":" + user->nick + " JOIN :" + channelName + "\r\n";
	for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
	{
		if (thisChannel->getMembersFd()[i] != user->socket_fd)
			sendMessage(thisChannel->getMembersFd()[i], msgJoinBroadcast);
	}
}