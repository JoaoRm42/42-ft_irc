#include "../libs.hpp"

void Server::welcomeMessage(Client& client)
{
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 001" + " : Welcome to the " +
				displayHostname() + " Network, " + client.getNick() + "!" +
				client.getUser() + "@" + getIP() + "\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 002" + " : Your host is ft_Irc, running version 1.302\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 003" + " : This server was created " + _creationServerTimeString + "\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 004 " + ": " + displayHostname() + " 1.302 " + "itkol " + "kol\r\n");
	sendMessage(client.getSocketFD(),
			   ":" + displayHostname() + " 005 " + " : MAXCHANNELS=50 :are supported by this server" + "\r\n");
}
