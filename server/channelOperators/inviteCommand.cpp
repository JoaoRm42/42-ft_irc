/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   inviteCommand.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macastan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:37:50 by macastan          #+#    #+#             */
/*   Updated: 2024/07/03 16:37:52 by macastan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../../libs.hpp"

void	Server::tryToInvite(std::string& channelName, Client *user, std::vector<std::string> tokens) {

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
		std::string msgNoSuchChannel = ":" + displayHostname() + " 403 " + user->getNick() + " " + channelName + " :No such channel\r\n";
		sendMessage(user->getSocketFD(), msgNoSuchChannel);
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
	//check if the invited nick is in the server
	Client	*invitedClient = _getUserClass(tokens[1]);
	if (invitedClient == NULL)
	{
		std::string msgNoSuchNick = ":" + displayHostname() + " 403 " + user->getNick() + " " + channelName + " :No such nick\r\n";
		sendMessage(user->getSocketFD(), msgNoSuchNick);
		return;
	}
	//check if the invited nick is already in the channel
	for (k = 0; k < thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == invitedClient->getNick())
		{
			std::string msgUserOnChannel = ":" + displayHostname() + " 443 " + user->getNick() + " " + invitedClient->getNick() + " " + channelName + " :is already on channel\r\n";
			sendMessage(user->getSocketFD(), msgUserOnChannel);
			return;
		}
	}
	//send the success invite message to the user who made the invitation
	std::string msgInviting = ":" + displayHostname() + " 341 " + user->getNick() + " " + invitedClient->getNick() + " " + channelName + "\r\n";
	sendMessage(user->getSocketFD(), msgInviting);
	//send the invite message to the invited user
	std::string	msgInvite = ":" + user->getNick() + " INVITE " + invitedClient->getNick() + " " + channelName + "\r\n";
	sendMessage(invitedClient->getSocketFD(), msgInvite);
	thisChannel->setListOfMembers(invitedClient);
	//send a JOIN message for the invited person enter the channel
	std::string	msgJoin = ":" + invitedClient->getNick() + " JOIN " + channelName + "\r\n";
	sendMessage(invitedClient->getSocketFD(), msgJoin);
}
