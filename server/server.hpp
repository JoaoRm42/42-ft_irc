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
# include "client_info.hpp"
# include "../channel/Channel.hpp"

class Client;

class	Channel;

class Server {
private:
    std::string _port;
    std::string _password;
    std::vector<Client> _clientInfo;
    std::map<int, Client*> _tmpClients;
    std::map<std::string, Channel *> _channelsList;
    std::map<int, std::string> _messages;

public:
    Server();
    Server(char **);
    Server(const Server &);
    Server &operator=(const Server &);
    ~Server();

    // Getters
    std::string getPort();
    std::string getPassword();

    std::string displayHostname();
    std::string getIP();
	uint16_t getPortAsUint16();
    void printCoolntro();
    int createUser();
    int bindUser(int, const struct sockaddr_in&);
    int listenUser(int);
    void handleNewConnection(int, int);
    void handleClientData(int);
    int epollFunction();
	int checkMessage(Client*, std::string);
    void sendChannelMessage(std::pair<std::vector<std::string>, std::string>, Client*);

	void	sendMessage(int, std::string);

	//channels functions
	bool	checkForOperators(std::string line, Client* user);
	void	tryToJoinChannel(std::string &channelName, Client *user, std::vector<std::string> tokens);
	void	joinExistingChannel(std::string channelName, Channel *thisChannel, Client *user, std::string channelPass, int flag);
	void	tryToPartChannel(std::string &channelName, Client *user, std::vector<std::string> tokens);
	void	partChannel(std::string channelName, Channel *thisChannel, Client *user, std::string reason, int flag);
	void	removeChannel(std::string);
	void	tryToKick(std::string&, Client *, std::vector<std::string>);
	void	kickChannel(std::string, Channel *, Client *, std::string, int, std::string);
};


#endif
