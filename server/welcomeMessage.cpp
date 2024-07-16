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
	sendWelcomeMessage(client);
}

void	Server::sendWelcomeMessage(Client &client) {
	//RPL_MOTDSTART (375)
	//"<client> :- <server> Message of the day - "
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 375 " + client.getNick() + " : - ft_Irc Message of the day - \r\n");
	//RPL_MOTD (372)
	//"<client> :<line of the motd>"
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 372 " + client.getNick() + " : " + "                                                         __\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 372 " + client.getNick() + " : " + "                                                         `\\\\\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 372 " + client.getNick() + " : " + "                  /\\-/\\          /\\-/\\          /\\=/\\-\"\"-.//\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 372 " + client.getNick() + " : " + "              /) = ^I^ =        = 'Y' = (\\     = 'Y' =  ,  \\\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 372 " + client.getNick() + " : " + "             ((_ /'-^-'\\ _     _/'-^-'\\_))      '-u-'  /(  /\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 372 " + client.getNick() + " : " + "              `\\`\\ \\ / /`/     \\\\ \\ / //`       /;_,) |\\\\ \\\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 372 " + client.getNick() + " : " + "                (_\\_|_/_)       (_(_|_)_)      (_/ (_/ (_(_/\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 372 " + client.getNick() + " : " + "                 \"\"\" \"\"\"         \"\"\" \"\"\"       \"\"  \"\"  \"\" \"\"\r\n");
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 372 " + client.getNick() + " : " + "                 macastan            joaoped2          ccosta-c\r\n");
	//RPL_ENDOFMOTD (376)
	//"<client> :End of /MOTD command."
	sendMessage(client.getSocketFD(),
				":" + displayHostname() + " 376 " + client.getNick() + " : End of /MOTD command.\r\n");
}