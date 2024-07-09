/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libs.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:26:24 by joaoped2          #+#    #+#             */
/*   Updated: 2024/07/01 18:53:30 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBS_HPP
# define LIBS_HPP

//System Libs
# include <iostream>
# include <string>
# include <sstream>
# include <sys/epoll.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <unistd.h>
# include <netdb.h>
# include <vector>
# include <sstream>
# include <netdb.h>
# include <cstring>
# include <arpa/inet.h>
# include <cstdlib>
# include <iomanip>
# include <map>
# include <stdint.h>
# include <algorithm>
# include <errno.h>
# include <fcntl.h>
# include <limits>

//Server Libs
# include "server/server.hpp"
# include "channel/Channel.hpp"
# include "utils/utils.hpp"
# include "client/Client.hpp"

#define LIMITOFCHANNELS	50

#define JOIN(nick, channel) \
	":" + std::string(nick) + " JOIN " + std::string(channel) + "\r\n"

#define JOINBROD(nick, channel) \
	":" + std::string(nick) + " JOIN :" + std::string(channel) + "\r\n"

#define MODE(nick, channel) \
	":" + std::string(nick) + " MODE " + std::string(channel) + " +t" + "\r\n"

#define NAMEREPLY(hostname, nick, symbol, channel, users) \
	":" + std::string(hostname) + " 353 " + std::string(nick) + " " + std::string(symbol) + " " + std::string(channel) + " :" + std::string(users) + "\r\n"

#define ENDOFLIST(hostname, nick, channel) \
	":" + std::string(hostname) + " 366 " + std::string(nick) + " " + std::string(channel) + " :End of /NAMES list.\r\n"

#define PRIVMSG(nick, channel, message) \
	":" + std::string(nick) + " PRIVMSG " + std::string(channel) + " :" + std::string(message) + "\r\n"

#endif
