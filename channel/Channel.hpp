/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: macastan <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:57:52 by macastan          #+#    #+#             */
/*   Updated: 2024/06/18 14:57:54 by macastan         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef	CHANNEL_HPP
# define CHANNEL_HPP

# include "../libs.hpp"
# include "../server/client_info.hpp"

class	Channel
{
private:
	std::string	_channelName;
	std::vector<std::string>	_listOfMembers;
	std::vector<std::string>	_listOfAdmins;
	std::vector<int>			_membersFd;
	std::string	_topic;
	std::string	_password;
	bool	inviteOnly;
	size_t	numOfMembers;
public:
	Channel(std::string name);
	~Channel();
	void	setListOfMembers(clientInfo& user);
	void	setListOfAdmins(clientInfo& user);
	std::vector<int>	getMenbersFd(void);
	int					getFirstFd(void);
};

#endif