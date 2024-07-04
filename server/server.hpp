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

class	Channel;

class Server {
private:
    std::string _port;
    std::string _password;
    std::vector<struct clientInfo> _clientInfo;
    std::map<int, struct clientInfo*> _tmpClients;
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
    void handleClientData(int, int);
    int epollFunction();
    std::vector<std::string> split(const std::string &, char);
    std::vector<std::string> splitspace(const std::string &);
    void initInput(std::pair<std::vector<std::string>, std::string>*, std::string);
    int checkSingle(clientInfo*, std::string);
	int check_message(clientInfo*, std::string);
    //void create_channel(const std::string& channel_name);
    //void add_user_to_channel(const std::string& channel_name, clientInfo& user);
    void sendChannelMessage(std::pair<std::vector<std::string>, std::string>, clientInfo*);
    void joinBroadcastChannel(std::string , Channel *, clientInfo *);

	void	sendMessage(int, std::string);
    void    fillInfo(clientInfo*);

	//channels funtions
	bool	checkForOperators(std::string, clientInfo*);
	void	tryToJoinChannel(std::string&, clientInfo*, std::vector<std::string>);
	void	joinExistingChannel(std::string, Channel *, clientInfo*, std::string, int);
	void	tryToPartChannel(std::string&, clientInfo*, std::vector<std::string>);
	void	partChannel(std::string, Channel *, clientInfo*, std::string, int);
	void	removeChannel(std::string);
};


#endif
