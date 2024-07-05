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
#include "../utils/utills.hpp"

bool	Server::checkForOperators(std::string line, clientInfo* user) {
	//Check the operator and send to the function
	std::vector<std::string> tokens = channelSplit(line);

//std::vector<std::string> result = channelSplit(line);
//for (std::vector<std::string>::iterator it = tokens.begin(); it != tokens.end(); it++) {
//	std::cout << *it << "\n";
//}

	if (tokens[0] == "JOIN" && tokens.size() > 1)
	{
		if (tokens[1][0] == '#' || tokens[1][0] == '&') {
			tryToJoinChannel(tokens[1], user, tokens);
		}
		else {
			if (tokens[1].find('\r') != std::string::npos)
				tokens[1].erase(tokens[1].find('\r'));
			std::string msgBadChanMask = ":" + displayHostname() + " 476 " + user->nick + " " + tokens[1] + " :Bad Channel Mask\r\n";
			sendMessage(user->socket_fd, msgBadChanMask);
		}
		return (true);
	}
	if (tokens[0] == "PART" && tokens.size() > 1)
	{
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
	return (false);
}