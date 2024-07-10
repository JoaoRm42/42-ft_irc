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
# include <cstdarg>

//Server Libs
# include "server/server.hpp"
# include "channel/Channel.hpp"
# include "utils/utils.hpp"
# include "client/Client.hpp"

#define LIMITOFCHANNELS	50

#define PRIVMSG(nick, channel, message) \
	":" + std::string(nick) + " PRIVMSG " + std::string(channel) + " :" + std::string(message) + "\r\n"

#endif
