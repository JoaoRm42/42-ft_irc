
# include "Client.hpp"

Client::Client() {
	socket_fd = 0;
	numOfChannels = 0;
	isValidData = false;
	isValidNick = false;
	validatedPass = false;
	validatedUser = false;
    isHexchat = false;
	alreadyWelcomed = false;
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

size_t Client::getNumOfChannels() const {
	return (numOfChannels);
}

bool Client::getValidData() const {
	return (isValidData);
}

bool Client::getAlreadyWelcomed() const {
	return alreadyWelcomed;
}

bool Client::getIsHexchat() {
    return (isHexchat);
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

void Client::setValidData(const bool data) {
	isValidData = data;
}

void Client::checkClientParams(Server& Server, const std::string& buffer) {
	std::vector<std::string> tmp = split(buffer, ' ');
	isValidData = false;
	if (tmp[0] == "PASS")
	{
		if (tmp.size() != 2 ) {
			Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 461 " + "PASS :Not enough parameters\r\n");
			return ;
		}
		if (validatedPass) {
			Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 462 " + ":You may not reregister\r\n");
			return ;
		}
		if (tmp[1] != Server.getPassword()) {
			Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 464 " + ":Password incorrect\r\n");
			return ;
		}
		validatedPass = true;
	}
	else if (tmp[0] == "NICK" && validatedPass) {
		checkNick(Server, tmp);
	}
	else if (tmp[0] == "USER" && validatedPass) {
		checkUser(Server, tmp);
		}
    else if (tmp[0] == "CAP") {
        isHexchat = true;
    }
	if (!nick.empty() && !user.empty() && isValidNick && validatedPass) {
		isValidData = true;
	}
}

bool Client::checkForbiddenChars(const std::string& toCheck) {
	if (toCheck.find_last_of("&$:~@%+",0) != toCheck.npos)
		return (true);
	if (toCheck.find_first_of(" ,*?!@.") != toCheck.npos)
		return (true);
	return (false);
}

void Client::checkNick(Server& Server, const std::vector<std::string> &tmp) {
	isValidNick = false;
	if (tmp.size() == 1 || (tmp.size() == 2 && tmp[1].empty())) {
		Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 431 " +  " :No nickname given\r\n");
		return ;
	}
	if (checkForbiddenChars(tmp[1])) {
		if (nick.empty()) {
			Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 432 " + tmp[1] + " " + tmp[1] + " :Erroneous nickname\r\n");
			nick = tmp[1];
			return ;
		}
		else {
			Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 432 " + nick + " " + tmp[1] + " :Erroneous nickname\r\n");
			return ;
		}
	}
	if(!Server.checkUniqueNick(tmp[1])) {
		if (nick.empty()) {
			Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 433 " + tmp[1] + " " + tmp[1] + " :Nickname is already in use\r\n");
			nick = tmp[1];
			return ;
		}
		else {
			Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 433 " + nick + " " + tmp[1] + " :Nickname is already in use\r\n");
			return ;
		}
	}
	Server.sendMessage(socket_fd, ":" + nick + " NICK " + tmp[1] + "\r\n");
	nick = tmp[1];
	isValidNick = true;
}

void Client::checkUser(Server& Server, const std::vector<std::string> &tmp) {
	if (validatedUser) {
		Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 462 " + ":You may not reregister\r\n");
		return ;
	}
	if (tmp.size() < 4) {
		Server.sendMessage(socket_fd, ":" + Server.displayHostname() + " 461 " + "USER :Not enough parameters\r\n");
		return;
	}
	if (tmp[2] == "0" && tmp[3] == "*") {
		user = tmp[1];
		validatedUser = true;
		return ;
	}
}

std::vector<std::string>	Client::getChannels() const {
	return (channels);
}

void Client::setAlreadyWelcomed(bool bollean) {
	Client::alreadyWelcomed = bollean;
}
