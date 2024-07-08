
# include "Client.hpp"

Client::Client() {
	socket_fd = 0;
	numOfChannels = 0;
}

Client::~Client() {}

int Client::getSocketFD() const {
	return (socket_fd);
}

std::string Client::getNick() const {
	return (nick);
}

std::string Client::getUser() const {
	return (user);
}

std::string Client::getPass() const {
	return (pass);
}

size_t Client::getNumOfChannels() const {
	return (numOfChannels);
}

void Client::setSocketFD(unsigned int data) {
	socket_fd = data;
}

void Client::setNick(std::string data) {
	nick = data;
}

void Client::setUser(std::string data) {
	user = data;
}

/// Adds a new data to the vector of channels that this client is a part of.
/// \param data The std::string of the channel to add.
/// \return	0 if it is successful or -1 if the numOfChannels is already at the limit defined bt LIMITOFCHANNELS in libs.hpp
int Client::addBackChannel(std::string data) {
	if (numOfChannels < LIMITOFCHANNELS) {
		channels.push_back(data);
		numOfChannels++;
		return (0);
	}
	return (-1);
}

void Client::setPass(std::string data) {
	pass = data;
}

bool Client::checkClientParams(std::string serverPassword, char *buffer) {
	std::vector<std::string> tmp = split(buffer, ' ');
	if (tmp[0] == "PASS")
	{
		if (tmp[1].empty())
		{
			std::cout << "Required Password!" << std::endl;
			return 1;
		}
		if (tmp[1] != serverPassword)
		{
			std::cout << "Wrong Password!" << std::endl;
			return 1;
		}
		pass= (tmp[1]);
		serverPassword.clear();
	}
	else if (tmp[0] == "NICK") {
		nick = tmp[1];
	}
	else if (tmp[0] == "USER") {
		user =  tmp[1];
	}
	return 0;
}
