# include "../libs.hpp"

void Server::createBotAndJoinChannel(const std::string& server, int port, const std::string& channel) {
	int sockfd;
	struct sockaddr_in server_addr;
	struct hostent *server_host;

	server_host = gethostbyname(server.c_str());
	if (server_host == NULL) {
		std::cerr << "Error, no such host" << std::endl;
		return;
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	this->_socketFdBot = sockfd;
	if (sockfd < 0) {
		std::cerr << "Error opening socket" << std::endl;
		return;
	}

	memset((char *) &server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	memcpy((char *) &server_addr.sin_addr.s_addr, (char *) server_host->h_addr, server_host->h_length);
	server_addr.sin_port = htons(port);

	if (connect(sockfd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		std::cerr << "Error connecting" << std::endl;
		close(sockfd);
		return;
	}

	std::string nick = "BOT";
	std::string user = "BOT 0 * :realname";
	std::string pass = getPassword();

	std::string msgNick = "NICK " + nick + "\r\n";
	std::string msgUser = "USER " + user + "\r\n";
	std::string msgPass = "PASS " + pass + "\r\n";
	std::string msgJoin = "JOIN " + channel + "\r\n";

	send(sockfd, msgNick.c_str(), msgNick.size(), 0);
	send(sockfd, msgUser.c_str(), msgUser.size(), 0);
	send(sockfd, msgPass.c_str(), msgPass.size(), 0);
	send(sockfd, msgJoin.c_str(), msgJoin.size(), 0);
}
