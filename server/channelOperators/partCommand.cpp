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

void	Server::tryToPartChannel(std::string& channelName, clientInfo* user, std::vector<std::string> tokens) {
	if (channelName.find('\r') != std::string::npos)
		channelName.erase(channelName.find('\r'));
	std::map<std::string, Channel *>::iterator it;
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
		std::string msgNotOnChannel = ":" + displayHostname() + " 403 " + user->nick + " " + channelName + " :No such channel\r\n";
		sendMessage(user->socket_fd, msgNotOnChannel);
		return;
	}
}

void	Server::partChannel(std::string channelName, Channel *thisChannel, clientInfo *user, std::string reason, int flag) {
	size_t k;
	for (k = 0; k < thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == user->nick)
		{
			break ;
		}
	}
	if (k == thisChannel->getlistOfMembers().size())
	{
		std::string msgNotOnChannel = ":" + displayHostname() + " 442 " + user->nick + " " + channelName + " :You're not on that channel\r\n";
		sendMessage(user->socket_fd, msgNotOnChannel);
		return;
	}
	(void)reason;
	(void)flag;

	std::string msgPart = ":" + user->nick + " PART " + channelName + "\r\n";
	sendMessage(user->socket_fd, msgPart);

	thisChannel->removeUser(user);

	//acho que nao esta a remover as cenas na funcao removeUser, ver amanha!!!!!!!

	std::string allMembers = thisChannel->getMembersForList();
	std::string msgNamReply = ":" + displayHostname() + " 353 " + user->nick + " = " + channelName + " :" + allMembers + "\r\n";
	sendMessage(user->socket_fd, msgNamReply);

	std::string msgEndOfList = ":" + displayHostname() + " 366 " + user->nick + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user->socket_fd, msgEndOfList);

	std::string msgPartBroadcast;
	if (flag == 0)
		msgPartBroadcast = ":" + user->nick + " PART :" + channelName + "\r\n";
	else
		msgPartBroadcast = ":" + user->nick + " PART :" + channelName + " " + reason + "\r\n";
	for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
	{
		if (thisChannel->getMembersFd()[i] != user->socket_fd)
			sendMessage(thisChannel->getMembersFd()[i], msgPartBroadcast);
	}
}