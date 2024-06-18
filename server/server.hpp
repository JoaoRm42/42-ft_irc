/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:28 by marvin            #+#    #+#             */
/*   Updated: 2024/06/18 17:19:15 by joaoped2         ###   ########.fr       */
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
		int bind_user(int serverSocket, const struct sockaddr_in& serverAddr);
		int listen_user(int serverSocket);
		struct sockaddr_in serveraddr();
		int create_epoll();
};

#endif
