/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   client_info.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 15:22:35 by joaoped2          #+#    #+#             */
/*   Updated: 2024/06/20 12:38:24 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef CLIENT_INFO_HPP
# define CLIENT_INFO_HPP

# include "../libs.hpp"

struct clientInfo {
	int socket_fd;
	std::string nick;
	std::string user;
	std::string pass;
	std::vector<std::string> channels;
	size_t numOfChannels;
};

#endif
