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

class Client;

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
	bool	_topicOn;
	bool	_limitOfUsers;
	size_t	_numOfMembers;
	size_t	_numMaxOfMembers;
	std::time_t	_creationTime;
	std::string	_creationTimeString;
	std::string _topicTimeString;
	std::time_t	_topicTime;
	std::string _topicNick;
	std::vector<std::string>	_invitedUsersToChannel;
public:
	Channel(std::string name);
	~Channel();
	void	removeBotFromChannel();
	void	setListOfMembers(Client *user);
	void	setListOfAdmins(Client *user);
	void	removeAdmin(Client *user);
	bool	isAdm(std::string userName);
	std::vector<std::string>	getlistOfMembers(void);
	std::vector<std::string>	getlistOfAdmins(void);
	std::vector<int>			getMembersFd(void);
	int							getFirstFd(void);
	bool						getInviteOnly(void);
	bool						getPasswordNeed(void);
	bool						getTopicOn(void);
	std::string					getPassword(void);
	size_t						getNumOfMembers(void);
	size_t						getNumMaxOfMembers(void);
	std::string					getMembersForList(void);
	void						removeUser(Client *user);
	int							getOneUserFd(std::string userName);
	void						removeUserKick(std::string userName, int fdUser);
	void						setInviteOnly(bool flag);
	std::string					getChannelName(void);
	void						setTopicOn(bool flag);
	void						setKeyPass(std::string key, bool keyOn);
	bool						getLimitOfUsers(void);
	void						setLimitOfUsers(size_t limit, bool limitOn);
	std::string					getAllModes(void);
	std::time_t					getCreationTime(void);
	void						setCrationTimeString(void);
	std::string					getCreationTimeString(void);
	std::string					getTopic(void);
	void						setTopic(std::string newTopic, std::string user);
	std::string					getTopicNick(void);
	std::string					getTopicTime(void);
	void						addInvitedUser(std::string userInvited);
	bool						checkListOfInvitedUsers(std::string userInvited);
	void						removeInvitedUser(std::string userInvited);
};

#endif
