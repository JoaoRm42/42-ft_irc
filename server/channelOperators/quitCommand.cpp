
#include "../server.hpp"
#include "../../libs.hpp"

void Server::quitCommand(std::vector<std::string> tokens, Client *user) {
	(void) tokens;
	close(user->getSocketFD());
	removeUsersChannels(user);
	sendMessage(user->getSocketFD(), ":" + user->getNick() + " ERROR " + ":Leaving\r\n");
	std::map<int, Client *>::iterator it = _tmpClients.find(user->getSocketFD());
	if (it != _tmpClients.end())
        delete it->second;
		_tmpClients.erase(it);

}

void	Server::removeUsersChannels(Client *user) {
	for (size_t k = 0; k < user->getChannels().size(); k++)
	{
		for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); ++it) {
			if (it->first == user->getChannels()[k])
			{
				it->second->removeUser(user);
				if (it->second->getNumOfMembers() == 0) {
					it->second->removeBotFromChannel();
					removeChannel(it->first);
				}
			}
		}
	}
}
