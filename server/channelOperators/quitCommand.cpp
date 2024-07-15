
#include "../server.hpp"
#include "../../libs.hpp"

void Server::quitCommand(std::vector<std::string> tokens, Client *user) {
	std::string msgQuit;
	if (tokens.size() == 1)
		msgQuit = ":" + user->getNick() + " QUIT " + "\r\n";
	else
		msgQuit = ":" + user->getNick() + " QUIT " + tokens[1] + "\r\n";
	removeUsersChannels(user, msgQuit);

	std::map<int, Client *>::iterator it = _tmpClients.find(user->getSocketFD());
	if (it != _tmpClients.end()) {
		delete it->second;
		_tmpClients.erase(it);
	}
	close(user->getSocketFD());
	std::cout << "Client Disconnected" << std::endl;
}

std::string	Server::getFirstUser() {
	std::map<std::string, Channel *>::iterator it = _channelsList.begin();
	return (it->second->getlistOfMembers()[0]);
}

void	Server::removeUsersChannels(Client *user, std::string quitMsg) {
	if (_channelsList.empty())
	{
		return;
	}
	else if (_tmpClients.size() == 2 && _channelsList.size() >= 1 && getFirstUser() == "BOT")
	{
		for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); ++it) {
			delete it->second;
		}
		_channelsList.clear();
		return;
	}
	else if (_channelsList.size() == 1 && _tmpClients.size() > 2)
	{
		std::map<std::string, Channel *>::iterator it = _channelsList.begin();
		if (it->second->getlistOfMembers().size() > 2)
		{
			for (size_t k = 0; k < it->second->getlistOfMembers().size(); k++)
			{
				if (it->second->getlistOfMembers()[k] == user->getNick())
				{
					it->second->removeUser(user);
					if (it->second->getNumOfMembers() > 1)
					{
						for (size_t i = 0; i < it->second->getMembersFd().size(); i++)
							sendMessage(it->second->getMembersFd()[i], quitMsg);
					}
				}
			}
		}
		return;
	}
	else {
		std::vector<Channel *> tmpChannelsToRemove;
		for (size_t k = 0; k < user->getChannels().size(); k++)
		{
			for (std::map<std::string, Channel *>::iterator it = _channelsList.begin(); it != _channelsList.end(); ++it) {
				if (it->first == user->getChannels()[k])
				{
					it->second->removeUser(user);
					if (it->second->getlistOfMembers().size() > 1)
					{
						for (size_t i = 0; i < it->second->getMembersFd().size(); i++)
							sendMessage(it->second->getMembersFd()[i], quitMsg);
					}
					if (it->second->getlistOfMembers().size() == 1 && it->second->getlistOfMembers()[0] == "BOT") {
						tmpChannelsToRemove.push_back(it->second);
					}
				}
			}
		}
		for (size_t c = 0; c < tmpChannelsToRemove.size(); c++)
		{
			tmpChannelsToRemove[c]->removeBotFromChannel();
			removeChannel(tmpChannelsToRemove[c]->getChannelName());
		}
	}
}
