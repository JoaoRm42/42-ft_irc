/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Channel.hpp                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 14:57:52 by macastan          #+#    #+#             */
/*   Updated: 2024/07/01 18:59:46 by joaoped2         ###   ########.fr       */
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
	bool	_inviteOnly;
	bool	_passwordNeed;
	size_t	_numOfMembers;
	size_t	_numMaxOfMembers;
public:
	Channel(std::string name);
	~Channel();
	void	setListOfMembers(clientInfo* user);
	void	setListOfAdmins(clientInfo* user);
	std::vector<std::string>	getlistOfMembers(void);
	std::vector<std::string>	getlistOfAdmins(void);
	std::vector<int>			getMembersFd(void);
	int							getFirstFd(void);
	bool						getInviteOnly(void);
	bool						getPasswordNeed(void);
	std::string					getPassword(void);
	size_t						getNumOfMembers(void);
	size_t						getNumMaxOfMembers(void);
	void						joinBroadcastChannel(clientInfo *, std::string);
	void						sendMessageChannel(int, std::string);
};

#endif
