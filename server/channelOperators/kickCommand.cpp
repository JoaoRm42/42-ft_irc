/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kickCommand.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macastan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:37:41 by macastan          #+#    #+#             */
/*   Updated: 2024/07/03 16:37:43 by macastan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../../libs.hpp"

void	Server::tryToKick(std::string& channelName, Client *user, std::vector<std::string> tokens){

	std::map<std::string, Channel *>::iterator it;
	for (it = _channelsList.begin(); it != _channelsList.end(); it++)
	{
		if (it->first == channelName)
		{
			if (tokens.size() > 3)
				kickChannel(channelName, it->second, user, tokens[3], 1, tokens[2]);
			else
			{
				std::string	noPass = "no reason";
				kickChannel(channelName, it->second, user, noPass, 0, tokens[2]);
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

void	Server::kickChannel(std::string channelName, Channel *thisChannel, Client *user, std::string reason, int flag, std::string kickedUser) {
	size_t k;
	//check se o membro esta neste canal
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
	//check se este membro é adm para poder dar kick
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

	//check se o membro que sera kickado esta no canal
	for (k = 0; k < thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == kickedUser)
			break ;
	}
	if (k == thisChannel->getlistOfMembers().size())
	{
		std::string msgUserNotOnChannel = ":" + displayHostname() + " 441 " + user->getNick() + " " + channelName + " :They aren't on that channel\r\n";
		sendMessage(user->getSocketFD(), msgUserNotOnChannel);
		return;
	}

	reason = "\"" + reason + "\"";
	std::string msgKick;
	if (flag == 0)
		msgKick = ":" + user->getNick() + " KICK " + channelName + " " + kickedUser + "\r\n";
	else
		msgKick = ":" + user->getNick() + " KICK " + channelName + " " + kickedUser + " :" + reason + "\r\n";
	for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
	{
		sendMessage(thisChannel->getMembersFd()[i], msgKick);
	}

	thisChannel->removeUserKick(kickedUser, thisChannel->getOneUserFd(kickedUser));
	if (thisChannel->getNumOfMembers() == 0)
	{
		removeChannel(channelName);
		return;
	}
}
