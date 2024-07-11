
#include "../server.hpp"
#include "../../libs.hpp"

void Server::quitCommand(std::vector<std::string> tokens, Client *user) {
	(void) tokens;
	sendMessage(user->getSocketFD(), ":" + user->getNick() + " ERROR " + ":Leaving\r\n");
	std::map<int, Client *>::iterator it = _tmpClients.find(user->getSocketFD());
	if (it != _tmpClients.end())
        delete it->second;
		_tmpClients.erase(it);
}
