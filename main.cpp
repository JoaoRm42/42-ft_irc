/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:12:43 by marvin            #+#    #+#             */
/*   Updated: 2024/06/17 15:50:43 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

void print_cool_intro() {
    std::cout << "┌──────────────────────────────────────────────────────────────────────────────────────────────┐\n";
    std::cout << "|  __________  __________  __________  __________  __________  __________  _________________   │\n";
    std::cout << "| ||F        |||T        |||_        |||I        |||R        |||C        |||                |  │\n";
    std::cout << "| ||_________|||_________|||_________|||_________|||_________|||_________|||________________|  │\n";
    std::cout << "| |/_________/|/_________/|/_________/|/_________/|/_________/|/_________/|/________________/  │\n";
    std::cout << "|                                                                                              │\n";
    std::cout << "└──────────────────────────────────────────────────────────────────────────────────────────────┘\n\n";
}


int main(int ac, char **av) {
    if (ac == 3)
    {
        Server teste(av);
        std::vector<Client_info> client;
        std::string option;
        print_cool_intro();
        int i = 0;
        while (1)
        {
            std::cin >> option;
            if (std::cin.eof())
                break;
            else if (option == "ADD") {
                std::stringstream ss;
                ss << "Client" << i;
                Client_info tmp;
                tmp.name = ss.str();
                tmp.admin = false;
                tmp.nick = "Cli";
                tmp.socket_fd = i;
                client.push_back(tmp);
                i++;
            }
            else if (option == "SHOW") {
                const Client_info& lastClient = client.back();
                std::cout << lastClient.name << std::endl;
                std::cout << lastClient.admin << std::endl;
                std::cout << lastClient.nick << std::endl;
                std::cout << lastClient.socket_fd << std::endl;
            }
       }
    }
    else
        std::cout << "Wrong usage!\nTry: ./ircserv <port> <password>" << std::endl;
    return (0);
}
