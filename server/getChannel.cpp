/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getChannel.cpp                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/20 12:39:05 by macastan          #+#    #+#             */
/*   Updated: 2024/07/01 19:02:49 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs.hpp"
#include "../utils/utils.hpp"

bool	Server::checkForOperators(std::string line, Client* user) {
	//Check the operator and send to the function
	std::vector<std::string> tokens = channelSplit(line);
	if (tokens[0] == "JOIN" && tokens.size() > 1)
	{
		if (tokens[1].find(',') != std::string::npos)
		{
			std::vector<std::string> channels = split(tokens[1], ',');
			std::vector<std::string> pass;
			if (tokens.size() > 2 && tokens[2].find(',') != std::string::npos)
				pass = split(tokens[2], ',');
			else if (tokens.size() > 2)
				pass.push_back(tokens[2]);
			for (size_t k = 0; k < channels.size(); k++)
			{
				tokens.clear();
				tokens.push_back("JOIN");
				tokens.push_back(channels[k]);
				if (k < pass.size())
					tokens.push_back(pass[k]);
				if (tokens[1][0] == '#' || tokens[1][0] == '&')
					tryToJoinChannel(tokens[1], user, tokens);
				else {
					std::string msgBadChanMask = ":" + displayHostname() + " 476 " + user->getNick() + " " + tokens[1] + " :Bad Channel Mask\r\n";
					sendMessage(user->getSocketFD(), msgBadChanMask);
				}
			}
		}
		else
		{
			if (tokens[1][0] == '#' || tokens[1][0] == '&')
				tryToJoinChannel(tokens[1], user, tokens);
			else {
				std::string msgBadChanMask = ":" + displayHostname() + " 476 " + user->getNick() + " " + tokens[1] + " :Bad Channel Mask\r\n";
				sendMessage(user->getSocketFD(), msgBadChanMask);
			}
		}
		return (true);
	}
	if (tokens[0] == "PART" && tokens.size() > 1)
	{
		if (tokens[1].find(',') != std::string::npos)
		{
			std::vector<std::string> channels = split(tokens[1], ',');
			std::vector<std::string> reason;
			if (tokens.size() > 2 && tokens[2].find(',') != std::string::npos)
				reason = split(tokens[2], ',');
			else if (tokens.size() > 2)
				reason.push_back(tokens[2]);
			for (size_t k = 0; k < channels.size(); k++)
			{
				tokens.clear();
				tokens.push_back("PART");
				tokens.push_back(channels[k]);
				if (k < reason.size())
					tokens.push_back(reason[k]);
				tryToPartChannel(tokens[1], user, tokens);
			}
		}
		else
			tryToPartChannel(tokens[1], user, tokens);
		return (true);
	}
	if (tokens[0] == "KICK" && tokens.size() > 1)
	{
		tryToKick(tokens[1], user, tokens);
		return (true);
	}
	/*if (tokens[0] == "INVITE" && tokens.size() > 1)
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
	return (true);
}
