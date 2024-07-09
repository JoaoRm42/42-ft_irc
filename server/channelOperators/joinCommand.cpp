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

void Server::createBotAndJoinChannel(const std::string& server, int port, const std::string& channel) {
	int sockfd;
	struct sockaddr_in server_addr;
	struct hostent *server_host;

	server_host = gethostbyname(server.c_str());
	if (server_host == NULL) {
		std::cerr << "Error, no such host" << std::endl;
		return;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	this->_socketFdBot = sockfd;
	if (sockfd < 0) {
		std::cerr << "Error opening socket" << std::endl;
		return;
	}

	memset((char *) &server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	memcpy((char *) &server_addr.sin_addr.s_addr, (char *) server_host->h_addr, server_host->h_length);
	server_addr.sin_port = htons(port);

	if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Error connecting" << std::endl;
		close(sockfd);
		return;
	}

	std::string nick = "BOT";
	std::string user = "BOT 0 * :realname";
	std::string pass = getPassword();

	std::string msgNick = "NICK " + nick + "\r\n";
	std::string msgUser = "USER " + user + "\r\n";
	std::string msgPass = "PASS " + pass + "\r\n";
	std::string msgJoin = "JOIN " + channel + "\r\n";

	send(sockfd, msgNick.c_str(), msgNick.size(), 0);
	send(sockfd, msgUser.c_str(), msgUser.size(), 0);
	send(sockfd, msgPass.c_str(), msgPass.size(), 0);
	send(sockfd, msgJoin.c_str(), msgJoin.size(), 0);

	// Listen to server responses (optional, for debugging purposes)
//	while (true) {
//		memset(buffer, 0, sizeof(buffer));
//		int n = read(sockfd, buffer, sizeof(buffer) - 1);
//		if (n <= 0) {
//			break;
//		}
//		std::cout << buffer << std::endl;
//	}

//	close(sockfd);
}

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
	//tirar debug depois
	if (user->addBackChannel(channelName) == -1)
		std::cout << "DEBUG: Client is at max channels" << std::endl;
	newChannel->setListOfMembers(user);
	newChannel->setListOfAdmins(user);

	// Add Bot ///////////////////
	createBotAndJoinChannel("127.0.0.1", 6667, channelName);
	//////////////////////////////


	std::string	msgJoin = ":" + user->getNick() + " JOIN " + channelName + "\r\n";
	sendMessage(user->getSocketFD(), msgJoin);

	//std::string msgMode = ":" + user->getNick() + " MODE " + channelName + " +t\r\n";
	//(user->getSocketFD(), msgMode);

	std::string msgNamReply = ":" + displayHostname() + " 353 " + user->getNick() + " = " + channelName + " :@" + user->getNick() + "\r\n";
	sendMessage(user->getSocketFD(), msgNamReply);

	std::string msgEndOfList = ":" + displayHostname() + " 366 " + user->getNick() + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user->getSocketFD(), msgEndOfList);
}

void	Server::joinExistingChannel(std::string channelName, Channel *thisChannel, Client *user, std::string channelPass, int flag) {
	for (size_t k = 0; k != thisChannel->getlistOfMembers().size(); k++)
	{
		if (thisChannel->getlistOfMembers()[k] == user->getNick())
			return;
	}
	if (thisChannel->getInviteOnly())
	{
		std::string msgInviteOnly = ":" + displayHostname() + " 473 " + user->getNick() + " " + channelName + " :Cannot join channel (+i)\r\n";
		sendMessage(user->getSocketFD(), msgInviteOnly);
		return;
	}
	if (thisChannel->getPasswordNeed())
	{
		if (flag == 0 || thisChannel->getPassword() != channelPass)
		{
			std::string msgWrongPassword = ":" + displayHostname() + " 475 " + user->getNick() + " " + channelName + " :Cannot join channel (+k)\r\n";
			sendMessage(user->getSocketFD(), msgWrongPassword);
			return;
		}
	}
	if (thisChannel->getNumOfMembers() >= thisChannel->getNumMaxOfMembers())
	{
		std::string msgChannelFull = ":" + displayHostname() + " 471 " + user->getNick() + " " + channelName + " :Cannot join channel (+l)\r\n";
		sendMessage(user->getSocketFD(), msgChannelFull);
		return;
	}
	//tirar debug depois
	if (user->addBackChannel(channelName) == -1)
		std::cout << "Debug: The user is alredy in the MAX channels" << std::endl;
	thisChannel->setListOfMembers(user);
	std::string allMembers = thisChannel->getMembersForList();

	std::string	msgJoin = ":" + user->getNick() + " JOIN " + channelName + "\r\n";
	sendMessage(user->getSocketFD(), msgJoin);

	//verificar necessidade de setar o mode a +t
	//std::string msgMode = ":" + user->getNick() + " MODE " + channelName + " +t\r\n";
	//sendMessage(user->getSocketFD(), msgMode);

	std::string msgNamReply = ":" + displayHostname() + " 353 " + user->getNick() + " = " + channelName + " :" + allMembers + "\r\n";
	sendMessage(user->getSocketFD(), msgNamReply);

	std::string msgEndOfList = ":" + displayHostname() + " 366 " + user->getNick() + " " + channelName + " :End of /NAMES list.\r\n";
	sendMessage(user->getSocketFD(), msgEndOfList);

	std::string msgJoinBroadcast = ":" + user->getNick() + " JOIN :" + channelName + "\r\n";
	for (size_t i = 0; i < thisChannel->getMembersFd().size(); i++)
	{
		if (thisChannel->getMembersFd()[i] != user->getSocketFD())
			sendMessage(thisChannel->getMembersFd()[i], msgJoinBroadcast);
	}
}
