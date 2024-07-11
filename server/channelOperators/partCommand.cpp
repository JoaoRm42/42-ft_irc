/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   partCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macastan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:37:31 by macastan          #+#    #+#             */
/*   Updated: 2024/07/03 16:37:33 by macastan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../../libs.hpp"

void	Server::tryToPartChannel(std::string& channelName, Client *user, std::vector<std::string> tokens) {
	std::map<std::string, Channel *>::iterator it;
	//check if the channel exists
	for (it = _channelsList.begin(); it != _channelsList.end(); it++)
	{
		if (it->first == channelName)
		{
			if (tokens.size() > 2)
				partChannel(channelName, it->second, user, tokens[2], 1);
			else
			{
				std::string	noPass = "no reason";
				partChannel(channelName, it->second, user, noPass, 0);
			}
			return ;
		}
	}
	if (it == _channelsList.end())
	{
		std::string msgNotOnChannel = ":" + displayHostname() + " 403 " + user->getNick() + " " + channelName + " :No such channel\r\n";
		sendMessage(user->getSocketFD(), msgNotOnChannel);
		return;
	}
}

void	Server::partChannel(std::string channelName, Channel *thisChannel, Client *user, std::string reason, int flag) {
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
	//put the reason on double quotes
	reason = "\"" + reason + "\"";
	std::string msgPart;
	if (flag == 0)
		msgPart = ":" + user->getNick() + " PART " + channelName + "\r\n";
	else
		msgPart = ":" + user->getNick() + " PART " + channelName + " :" + reason + "\r\n";
	//send the message to everyone saying that the user has left the channel
	for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
	{
		sendMessage(thisChannel->getMembersFd()[i], msgPart);
	}
	//remove the user and check if the channel is empty, if it is remove the channel
	thisChannel->removeUser(user);
	if (thisChannel->getNumOfMembers() == 1 && thisChannel->getlistOfMembers()[0] == "BOT")
	{
		thisChannel->removeBotFromChannel();
		removeChannel(channelName);
//		close(getSocketFdBot());
		return;
	}
}

void	Server::removeChannel(std::string channelName) {
	for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); ++it) {
		if (it->first == channelName)
		{
			delete it->second;
			_channelsList.erase(it);
			break;
		}
	}
}
