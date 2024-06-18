/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   getHostname.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/06/18 11:13:02 by joaoped2          #+#    #+#             */
/*   Updated: 2024/06/18 12:20:45 by joaoped2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../libs.hpp"

std::string Server::display_hostname() {
	char hostname[256];

	memset(hostname, 0, sizeof(hostname));
	if (gethostname(hostname, sizeof(hostname)) == -1) {
		std::cerr << "Error: Unable to get the hostname" << std::endl;
		return ("0");
	}
	std::string host = hostname;
	return (host);
}

std::string Server::getIP() {
	std::string tmp = display_hostname();
	const char *hostname = tmp.c_str();
    struct addrinfo hints, *res, *p;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int status = getaddrinfo(hostname, 0, &hints, &res);
    if (status != 0) {
        std::cerr << "getaddrinfo error: " << gai_strerror(status) << std::endl;
        return ("0");
    }

	std::string tmp2;
    for (p = res; p != 0; p = p->ai_next) {
        void* addr;
        const char* ipver;
        char ipstr[INET6_ADDRSTRLEN];
        if (p->ai_family == AF_INET) {
            struct sockaddr_in* ipv4 = (struct sockaddr_in*)p->ai_addr;
            addr = &(ipv4->sin_addr);
            ipver = "IPv4";
        } else {
            struct sockaddr_in6* ipv6 = (struct sockaddr_in6*)p->ai_addr;
            addr = &(ipv6->sin6_addr);
            ipver = "IPv6";
        }
        inet_ntop(p->ai_family, addr, ipstr, sizeof ipstr);
		tmp2 = ipstr;
		break ;
    }
    freeaddrinfo(res);
	return (tmp2);
}
