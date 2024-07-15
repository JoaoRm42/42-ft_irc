/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:28 by marvin            #+#    #+#             */
/*   Updated: 2024/07/01 18:25:10 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define MAX_EVENTS 10

# include "../libs.hpp"
# include "../channel/Channel.hpp"

class Client;

class	Channel;

class Server {
	private:
		std::string	_port;
		std::string							_password;
		std::map<int, Client*>				_tmpClients;
		std::map<std::string, Channel *>	_channelsList;
		std::map<int, std::string>			_messages;
		int									_socketFdBot;

public:
	Server();
	Server(char **);
	Server(const Server &);
	Server &operator=(const Server &);
	~Server();

	// Getters
	std::string	getPort();
	std::string	getPassword();
	int 		getSocketFdBot();

	std::string	displayHostname();
	std::string	getIP();
	uint16_t	getPortAsUint16();
	void		printCoolntro();
	int			createUser();
	int			bindUser(int, const struct sockaddr_in&);
	int			listenUser(int);
	void		handleNewConnection(int, int);
	void		handleClientData(int);
	int			epollFunction();
	int			checkMessage(Client*, std::string);
	void 		sendChannelMessage(std::pair<std::vector<std::string>, std::string>, Client*);

	void	sendMessage(int, std::string);
	bool	checkUniqueNick(const std::string& toCheck);
	Client *_getUserClass(const std::string&);

	//bot functions
	void		BotJoinChannel(const std::string& channel);
	void		createBot(const std::string& server, int port);
	void		BotSendAsciiArt(const std::string& userNick);
	std::string	BotJokes();
    void        sendHelpTableBot(Client *user);

	//channels functions
	bool		checkForOperators(std::string line, Client *user, std::pair<std::vector<std::string>, std::string > input);
	void		tryToJoinChannel(std::string &channelName, Client *user, std::vector<std::string> tokens);
	void		joinExistingChannel(std::string channelName, Channel *thisChannel, Client *user, std::string channelPass, int flag);
	void		tryToPartChannel(std::string &channelName, Client *user, std::vector<std::string> tokens);
	void		partChannel(std::string channelName, Channel *thisChannel, Client *user, std::string reason, int flag);
	void		removeChannel(std::string);
	void		tryToKick(std::string&, Client *, std::vector<std::string>);
	void		kickChannel(std::string, Channel *, Client *, std::string, int, std::string);
	void		tryToMode(std::string&, Client *, std::vector<std::string>);
	void		modeChannel(Client *, std::vector<std::string>, Channel *);
	void		inviteMode(int, Channel *, Client *);
	void		topicMode(int, Channel *, Client *);
	void		keyMode(int, Channel *, Client *, std::string);
	void		limitUserMode(int, Channel *, Client *, std::string);
	void		showMode(std::string&, Client *);
	void		modeUser(Client *, std::vector<std::string>, Channel *);
	void		tryTopic(std::string&, Client *, std::vector<std::string>);
	void		seeTopic(Channel *, Client *);
	void		setTopic(Channel *, Client *, std::string);
	void		removeTopic(Channel *, Client *);
	void		tryToInvite(std::string& , Client *, std::vector<std::string>);

	//client funtions
	void		quitCommand(std::vector<std::string> tokens, Client *user);
	void		removeUsersChannels(Client *user, std::string);
	std::string	getFirstUser();
};


#endif
