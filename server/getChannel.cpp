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
	// nao esta a chegar as informacoes do user
	std::cout << "este eh o nick do user aqui: " << user.nick << std::endl;
	std::vector<std::string> tokens = split(line, ' ');
	if (tokens[0] == "JOIN" && tokens.size() > 1)
	{
		//std::cout << "join command\n";
		//std::cout << line << std::endl;
		tryToJoinChannel(tokens[1], user);
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

void Server::tryToJoinChannel(std::string& channelName, clientInfo& user) {
	// Check if the channel exists, create if it does not
	//std::map<std::string, Channel>::iterator it = _channelsList.begin();

	if (channelName.find('\r') != std::string::npos)
		channelName.erase(channelName.find('\r'));
	std::string	msg = ":" + user.nick + " JOIN " + channelName + "\r\n";
	//std::string msg = ":" + user.nick + "!d@localhost JOIN " + channelName + "\r\n";
	for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); it++)
	{
		if (it->first == channelName)
		{
			std::cout << it->first.size() << " e " << channelName.size() << std::endl;
			user.channels.push_back(channelName);
			it->second->setListOfMembers(user);
			//joinExistingChannel(it->second, user);
			sendMessage(user.socket_fd, msg);
			return ;
		}
	}
	Channel* newChannel = new Channel(channelName);
	//_channelsList.insert(channelName, newChannel);
	_channelsList[channelName] = newChannel;
	//adicionar channel ao cliente
	user.channels.push_back(channelName);
	newChannel->setListOfMembers(user);
	newChannel->setListOfAdmins(user);

	/*for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); it++)
	{
		std::cout << it->first << " and " << it->second << " and " << it->second->getFirstFd() <<  std::endl;
	}*/

	std::cout << "este eh o nick do user: " << user.nick << std::endl;
	sendMessage(user.socket_fd, msg);

	/*std::string msg2 = ":" + user.nick + " MODE " + channelName + " +t" + std::string() + "\r\n";
	sendMessage(user.socket_fd, msg2);

	std::string msg3 = ":" + displayHostname() + " 353 " + user.nick + " = " + channelName + " :" + user.nick + "\r\n";
	sendMessage(user.socket_fd, msg3);

	std::string msg4 = ":" + displayHostname() + " 366 " + user.nick + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user.socket_fd, msg4);*/
}
