/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:28 by marvin            #+#    #+#             */
/*   Updated: 2024/06/21 13:23:04 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define MAX_EVENTS 1000

# include "../libs.hpp"
# include "client_info.hpp"
# include "../channel/channel.hpp"

class Server {
private:
    std::string _port;
    std::string _password;
    std::vector<struct clientInfo> _clientInfo;
    std::map<std::string, Channel> channels;

public:
    Server();
    Server(char **av);
    Server(const Server &obj);
    Server &operator=(const Server &obj);
    ~Server();

    // Getters
    std::string getPort();
    std::string getPassword();

    std::string displayHostname();
    std::string getIP();
	uint16_t getPortAsUint16();
    void printCoolntro();
    int createUser();
    int bindUser(int sockfd, const struct sockaddr_in& server_addr);
    int listenUser(int sockfd);
    void handleNewConnection(int epoll_fd, int sockfd);
    void handleClientData(int epoll_fd, int clientSocket);
    int epollFunction();
    std::vector<std::string> split(const std::string &str, char delimiter);
    int checkSingle(clientInfo& clientInfo, const std::string& result);
	int check_message(clientInfo& client_info, char* buffer);
    void add_user_to_channel(const std::string& channel_name, clientInfo& user);
    void send_msg(int socket_fd, const char* msg);
};


#endif
