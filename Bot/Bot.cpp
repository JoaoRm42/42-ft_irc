# include "../libs.hpp"

void Server::BotJoinChannel(const std::string& channel) {
	std::string msgJoin = "JOIN " + channel + "\r\n";
	send(this->_socketFdBot , msgJoin.c_str(), msgJoin.size(), MSG_DONTWAIT);
}

void Server::createBot(const std::string& server, int port) {
	struct sockaddr_in server_addr;
	struct hostent *server_host;

	server_host = gethostbyname(server.c_str());
	if (server_host == NULL) {
		std::cerr << "Error, no such host" << std::endl;
		return;
	}

	this->_socketFdBot  = socket(AF_INET, SOCK_STREAM, 0);
	if (this->_socketFdBot  < 0) {
		std::cerr << "Error opening socket" << std::endl;
		return;
	}

	memset((char *) &server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	memcpy((char *) &server_addr.sin_addr.s_addr, (char *) server_host->h_addr, server_host->h_length);
	server_addr.sin_port = htons(port);

	if (connect(this->_socketFdBot , (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Error connecting" << std::endl;
		close(this->_socketFdBot );
		return;
	}

	std::string nick = "BOT";
	std::string user = "BOT 0 * :realname";
	std::string pass = getPassword();

	std::string msgNick = "NICK " + nick + "\r\n";
	std::string msgUser = "USER " + user + "\r\n";
	std::string msgPass = "PASS " + pass + "\r\n";

	send(this->_socketFdBot , msgNick.c_str(), msgNick.size(), MSG_DONTWAIT);
	send(this->_socketFdBot , msgUser.c_str(), msgUser.size(), MSG_DONTWAIT);
	send(this->_socketFdBot , msgPass.c_str(), msgPass.size(), MSG_DONTWAIT);
}
