/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getChannel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macastan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:39:05 by macastan          #+#    #+#             */
/*   Updated: 2024/06/20 12:39:06 by macastan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs.hpp"

bool	Server::checkForOperators(std::string line, clientInfo& user) {
	//Check the operator and send to the function
	std::vector<std::string> tokens = split(line, ' ');
	if (tokens[0] == "JOIN" && tokens.size() > 1)
	{
		if (tokens[1][0] == '#' || tokens[1][0] == '&') {
			tryToJoinChannel(tokens[1], user, tokens);
		}
		else {
			if (tokens[1].find('\r') != std::string::npos)
				tokens[1].erase(tokens[1].find('\r'));
			std::string msgBadChanMask = ":" + displayHostname() + " 476 " + user.nick + " " + tokens[1] + " :Bad Channel Mask\r\n";
			sendMessage(user.socket_fd, msgBadChanMask);
		}
		return (true);
	}
	/*if (tokens[0] == "PART" && tokens.size() > 1)
	{
		std::cout << "part command\n";
		return (true);
	}
	if (tokens[0] == "KICK" && tokens.size() > 1)
	{
		std::cout << "kick command\n";
		return (true);
	}
	if (tokens[0] == "INVITE" && tokens.size() > 1)
	{
		std::cout << "invite command\n";
		return (true);
	}
	if (tokens[0] == "TOPIC" && tokens.size() > 1)
	{
		std::cout << "topic command\n";
		return (true);
	}
	if (tokens[0] == "MODE" && tokens.size() > 1)
	{
		std::cout << "mode command\n";
		return (true);
	}*/
	return (false);
}

void Server::tryToJoinChannel(std::string& channelName, clientInfo& user, std::vector<std::string> tokens) {
	// Check if the channel exists, create if it does not

	if (user.numOfChannels >= LIMITOFCHANNELS)
	{
		std::string msgTooManyChannels = ":" + displayHostname() + " 405 " + user.nick + " " + channelName + " :You have joined too many channels\r\n";
		sendMessage(user.socket_fd, msgTooManyChannels);
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
			//joinExistingChannel(channelName, it->second, user);
			return ;
		}
	}

	Channel* newChannel = new Channel(channelName);
	_channelsList[channelName] = newChannel;
	//adicionar channel ao cliente e adicionar ++ ao num de canais
	user.channels.push_back(channelName);
	user.numOfChannels++;
	newChannel->setListOfMembers(user);
	newChannel->setListOfAdmins(user);

	std::string	msgJoin = ":" + user.nick + " JOIN " + channelName + "\r\n";
	sendMessage(user.socket_fd, msgJoin);

	//verificar necessidade de setar o mode a +t
	std::string msgMode = ":" + user.nick + " MODE " + channelName + " +t" + std::string() + "\r\n";
	sendMessage(user.socket_fd, msgMode);

	std::string msgNamReply = ":" + displayHostname() + " 353 " + user.nick + " = " + channelName + " :" + user.nick + "\r\n";
	sendMessage(user.socket_fd, msgNamReply);

	std::string msgEndOfList = ":" + displayHostname() + " 366 " + user.nick + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user.socket_fd, msgEndOfList);
}

void	Server::joinExistingChannel(std::string channelName, Channel *thisChannel, clientInfo &user, std::string channelPass, int flag) {
	//check se ja eh membro FALTA FAZER

	if (thisChannel->getInviteOnly() == true)
	{
		std::string msgInviteOnly = ":" + displayHostname() + " 473 " + user.nick + " " + channelName + " :Cannot join channel (+i)\r\n";
		sendMessage(user.socket_fd, msgInviteOnly);
		return;
	}
	if (thisChannel->getPasswordNeed() == true)
	{
		if (channelPass.find('\r') != std::string::npos)
			channelPass.erase(channelPass.find('\r'));
		if (flag == 0 || thisChannel->getPassword() != channelPass)
		{
			std::string msgWrongPassword = ":" + displayHostname() + " 475 " + user.nick + " " + channelName + " :Cannot join channel (+k)\r\n";
			sendMessage(user.socket_fd, msgWrongPassword);
			return;
		}
	}
	if (thisChannel->getNumOfMembers() >= thisChannel->getNumMaxOfMembers())
	{
		std::string msgChannelFull = ":" + displayHostname() + " 471 " + user.nick + " " + channelName + " :Cannot join channel (+l)\r\n";
		sendMessage(user.socket_fd, msgChannelFull);
		return;
	}

	user.channels.push_back(channelName);
	user.numOfChannels++;
	thisChannel->setListOfMembers(user);

	std::string	msgJoin = ":" + user.nick + " JOIN " + channelName + "\r\n";
	sendMessage(user.socket_fd, msgJoin);

	//verificar necessidade de setar os mode

	std::string msgNamReply = ":" + displayHostname() + " 353 " + user.nick + " = " + channelName + " :" + user.nick + "\r\n";
	sendMessage(user.socket_fd, msgNamReply);

	std::string msgEndOfList = ":" + displayHostname() + " 366 " + user.nick + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user.socket_fd, msgEndOfList);
}