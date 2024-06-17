/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.hpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: marvin <marvin@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:11:28 by marvin            #+#    #+#             */
/*   Updated: 2024/06/17 11:11:28 by marvin           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SERVER_HPP
# define SERVER_HPP

# include <iostream>
# include <string>
# include <sys/epoll.h>

class Server {
    private:
        std::string _port;
        std::string _password;
    public:
        Server();
        Server(char **av);
        Server(const Server &obj);
        Server &operator=(const Server &obj);
        ~Server();

        
        void print_private();
};

#endif