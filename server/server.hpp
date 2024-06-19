/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:28 by marvin            #+#    #+#             */
/*   Updated: 2024/06/19 15:31:38 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP
# define MAX_EVENTS 10

# include "../libs.hpp"
# include "client_info.hpp"

class Server {
	private:
		std::string _port;
		std::string _password;
		std::vector<struct Client_info> _client_info;

	public:
		Server();
		Server(char **av);
		Server(const Server &obj);
		Server &operator=(const Server &obj);
		~Server();

		//getters
		std::string get_port();
		std::string get_password();

		std::string display_hostname();
		std::string getIP();
		void print_cool_intro();
		int create_user();
		int epollFunction();
		uint16_t get_port_as_uint16();
		int bind_user(int sockfd, const struct sockaddr_in& server_addr);
		void handle_new_connection(int epoll_fd, int sockfd);
		void handle_client_data(int epoll_fd, int clientSocket);
		int check_single(Client_info& client_info, const std::string& result);
		int check_message(Client_info& client_info, char* buffer);
		int listen_user(int sockfd);
};

#endif
