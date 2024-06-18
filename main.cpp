/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/17 11:12:43 by marvin            #+#    #+#             */
/*   Updated: 2024/06/18 14:08:03 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libs.hpp"

// 94 Lengh

void print_line(const std::string& label, const std::string& value) {
    int valuelen = value.length();
    int total = (94 - 39 -  valuelen + 1);
    std::cout << "│" << std::setw(39) << label << ": " << value << std::setw(total) << "│" << std::endl;
}

void Server::print_cool_intro() {
    system("clear");
    std::string string = display_hostname();
    std::string ip = getIP();
    std::string port = get_port();
    std::string pass = get_password();
    std::cout << "┌──────────────────────────────────────────────────────────────────────────────────────────────┐\n";
    std::cout << "│  __________  __________  __________  __________  __________  __________  _________________   │\n";
    std::cout << "│ ||F        |||T        |||_        |||I        |||R        |||C        |||                |  │\n";
    std::cout << "│ ||_________|||_________|||_________|||_________|||_________|||_________|||________________|  │\n";
    std::cout << "│ |/_________/|/_________/|/_________/|/_________/|/_________/|/_________/|/________________/  │\n";
    std::cout << "│                                                                                              │\n";
    print_line("HOST", string);
    print_line("IPV4", ip);
    print_line("PORT", port);
    print_line("PASS", pass);
    std::cout << "└──────────────────────────────────────────────────────────────────────────────────────────────┘\n\n";
}


int main(int ac, char **av) {
    if (ac == 3)
    {
        Server teste(av);
        std::vector<Client_info> client;
        teste.print_cool_intro();
        teste.epollFunction();
    }
    else
        std::cout << "Wrong usage!\nTry: ./ircserv <port> <password>" << std::endl;
    return (0);
}
