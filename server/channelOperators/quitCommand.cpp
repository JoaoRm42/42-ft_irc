
#include "../server.hpp"
#include "../../libs.hpp"

void Server::quitCommand(std::vector<std::string> tokens, Client *user) {
	(void)tokens;
	sendMessage(user->getSocketFD(), ":" + user->getNick()+ " ERROR " + ":Leaving\r\n");
}