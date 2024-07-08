
# include "Client.hpp"

Client::Client() {
	socket_fd = 0;
	numOfChannels = 0;
	isValidData = 0;
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

bool Client::getValidData() const {
	return (isValidData);
}

void Client::setSocketFD(unsigned int data) {
	socket_fd = data;
}

void Client::setNick(const std::string& data) {
	nick = data;
}

void Client::setUser(const std::string& data) {
	user = data;
}

/// Adds a new data to the vector of channels that this client is a part of.
/// \param data The std::string of the channel to add.
/// \return	0 if it is successful or -1 if the numOfChannels is already at the limit defined bt LIMITOFCHANNELS in libs.hpp
int Client::addBackChannel(const std::string& data) {
	if (numOfChannels < LIMITOFCHANNELS) {
		channels.push_back(data);
		numOfChannels++;
		return (0);
	}
	return (-1);
}

void Client::setPass(const std::string& data) {
	pass = data;
}

void Client::setValidData(const bool data) {
	isValidData = data;
}

bool Client::checkClientParams(Server& Server, const std::string& buffer) {
	std::vector<std::string> tmp = split(buffer, ' ');
	setValidData(false);
	if (tmp[0] == "PASS")
	{
		if (!pass.empty()) {
			std::cout << "Password already set!" << std::endl;
			return (true);
		}
		if (tmp[1].empty()) {
			std::cout << "Required Password!" << std::endl;
			return (true);
		}
		if (tmp[1] != Server.getPassword()) {
			std::cout << "Wrong Password!" << std::endl;
			return (true);
		}
		pass = (tmp[1]);
		return (true);
	}
	else if (tmp[0] == "NICK") {
		if (tmp.size() == 1) {
			std::cout << "Required Nickname!" << std::endl;
			return (true);
		}
		if (tmp[1] == "ola" || tmp[1] == "adeus") {
			Server.sendMessage(socket_fd, "ERROR :Invalid Nickname\n");
			return (true);
		}
		nick = tmp[1];
		std::cout << "Nick Set!" << std::endl;
		return (true);
	}
	else if (tmp[0] == "USER") {
		if (!user.empty()) { {
				std::cout << "Entered here!" << std::endl;
				std::cout << "User already set!" << std::endl;
			}
			setValidData(false);
			return (true);
		}
		else {
			if (tmp.size() == 1){
				std::cout << "Required Username!" << std::endl;
				return (true);
			}
			user =  tmp[1];
			return (true);
		}
	}
	if (pass == Server.getPassword() && !nick.empty() && !user.empty())
		setValidData(true);
	return (false);
}
