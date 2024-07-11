
#include "../server.hpp"
#include "../../libs.hpp"

void Server::quitCommand(std::vector<std::string> tokens, Client *user) {
	(void) tokens;
	sendMessage(user->getSocketFD(), ":" + user->getNick() + " ERROR " + ":Leaving\r\n");
    for (std::map<std::string, Channel*>::iterator it = _channelsList.begin(); it != _channelsList.end();) {
        Channel* channel = it->second;
        channel->removeUser(user);
        if (channel->getNumOfMembers() == 1 &&  channel->getMembersForList() == "BOT") {
            channel->removeBotFromChannel();
            delete it->second;

            std::map<std::string, Channel*>::iterator eraseIt = it;
            ++it;
            _channelsList.erase(eraseIt);
        }
        else
            ++it;
    }
	std::map<int, Client *>::iterator it = _tmpClients.find(user->getSocketFD());
	if (it != _tmpClients.end())
        delete it->second;
		_tmpClients.erase(it);

}
