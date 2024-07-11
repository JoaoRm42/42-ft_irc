/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   topicCommand.cpp                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macastan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/03 16:37:58 by macastan          #+#    #+#             */
/*   Updated: 2024/07/03 16:38:00 by macastan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../server.hpp"
#include "../../libs.hpp"

void	Server::tryTopic(std::string &channelName, Client *user, std::vector<std::string> tokens) {
	std::map<std::string, Channel *>::iterator it;
	Channel *thisChannel;
	//check if the channel exists
	for (it = _channelsList.begin(); it != _channelsList.end(); it++)
	{
		if (it->first == channelName)
		{
			thisChannel	= it->second;
			break ;
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
	//chekc if the topic mode is seted, if it's not give the error
	if (thisChannel->getTopicOn() == false)
	{
		std::string msgNoTopic = ":" + displayHostname() + " 331 " + user->getNick() + " " + thisChannel->getChannelName() + " :No topic is set\r\n";
		sendMessage(user->getSocketFD(), msgNoTopic);
		return;
	}
	//divide the topic in the types: see, remove or set
	if (tokens.size() == 2)
		seeTopic(thisChannel, user);
	else if (tokens.size() >= 3)
	{
		if (tokens[2].size() == 1 && tokens[2][0] == ':')
			removeTopic(thisChannel, user);
		else
			setTopic(thisChannel, user, tokens[2]);
	}
}

void	Server::setTopic(Channel *thisChannel, Client *user, std::string newTopic) {
	thisChannel->setTopic(newTopic, user->getNick());
	std::string	msgTopic = ":" + user->getNick() + " TOPIC " + thisChannel->getChannelName() + " " + thisChannel->getTopic() + "\r\n";
	for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
		sendMessage(thisChannel->getMembersFd()[i], msgTopic);
}

void	Server::removeTopic(Channel *thisChannel, Client *user) {
	thisChannel->setTopic("", user->getNick());
	std::string	msgTopic = ":" + user->getNick() + " TOPIC " + thisChannel->getChannelName() + " " + thisChannel->getTopic() + "\r\n";
	for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
		sendMessage(thisChannel->getMembersFd()[i], msgTopic);
}

void	Server::seeTopic(Channel *thisChannel, Client *user) {
	if (thisChannel->getTopic().empty())
	{
		std::string msgNoTopic = ":" + displayHostname() + " 331 " + user->getNick() + " " + thisChannel->getChannelName() + " :No topic is set\r\n";
		sendMessage(user->getSocketFD(), msgNoTopic);
		return;
	}
	else
	{
		std::string msgTopic = ":" + displayHostname() + " 332 " + user->getNick() + " " + thisChannel->getChannelName() + " " + thisChannel->getTopic() + "\r\n";
		sendMessage(user->getSocketFD(), msgTopic);
		std::string msgTopicWhoTime = ":" + displayHostname() + " 333 " + user->getNick() + " " + thisChannel->getChannelName() + " " + thisChannel->getTopicNick() + " " + thisChannel->getTopicTime() + "\r\n";
		sendMessage(user->getSocketFD(), msgTopicWhoTime);
	}
}