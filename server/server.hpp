/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:28 by marvin            #+#    #+#             */
/*   Updated: 2024/06/17 15:36:47 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

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

		void create_user();
};

#endif
