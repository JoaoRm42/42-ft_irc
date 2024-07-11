
#include "../server.hpp"
#include "../../libs.hpp"

void Server::quitCommand(std::vector<std::string> tokens, Client *user) {
	(void) tokens;
	close(user->getSocketFD());
	sendMessage(user->getSocketFD(), ":" + user->getNick() + " ERROR " + ":Leaving\r\n");
	//:dan-!d@localhost QUIT :Quit: Bye for now!
	std::string msgQuit;
	if (tokens.size() == 1)
		msgQuit = ":" + user->getNick() + " QUIT " + "\r\n";
	else
		msgQuit = ":" + user->getNick() + " QUIT " + tokens[1] + "\r\n";;
	removeUsersChannels(user, msgQuit);
	std::map<int, Client *>::iterator it = _tmpClients.find(user->getSocketFD());
	if (it != _tmpClients.end())
        delete it->second;
		_tmpClients.erase(it);

}

void	Server::removeUsersChannels(Client *user, std::string quitMsg) {
	for (size_t k = 0; k < user->getChannels().size(); k++)
	{
		for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); ++it) {
			if (it->first == user->getChannels()[k])
			{
				it->second->removeUser(user);
				if (it->second->getNumOfMembers() > 1)
				{
					for (size_t i = 0; i < it->second->getMembersFd().size(); i++)
						sendMessage(it->second->getMembersFd()[i], quitMsg);
				}
				if (it->second->getNumOfMembers() == 1 && it->second->getlistOfMembers()[0] == "BOT") {
					it->second->removeBotFromChannel();
					removeChannel(it->first);
				}
			}
		}
	}
}
