# include "../libs.hpp"

void Server::BotSendAsciiArt(const std::string& userNick) {
	std::vector<std::string> asciiArtLines;

	asciiArtLines.push_back("              , ,");
	asciiArtLines.push_back("             /| |\\");
	asciiArtLines.push_back("            / | | \\");
	asciiArtLines.push_back("            | | | |     Nice try jackass!");
	asciiArtLines.push_back("            \\ | | /");
	asciiArtLines.push_back("             \\|w|/    /");
	asciiArtLines.push_back("             /_ _\\   /      , ");
	asciiArtLines.push_back("  /\\       _:()_():_       /]");
	asciiArtLines.push_back("  ||_     : ._=Y=_  :     / /");
	asciiArtLines.push_back(" [)(_,    ',__\\W/ _,'    /  \\");
	asciiArtLines.push_back(" [) \\_/\\    _/'='\\      /-/\\)");
	asciiArtLines.push_back("  [_| \\ \\  ///  \\ '._  / /");
	asciiArtLines.push_back("  :;   \\ \\///   / |  '` /");
	asciiArtLines.push_back("  ;::   \\ `|:   : |',_.'");
	asciiArtLines.push_back("  \"\"\"    \\_|:   : |   ");
	asciiArtLines.push_back("           |:   : |'\".");
	asciiArtLines.push_back("           /`._.'  \\");
	asciiArtLines.push_back("          /  /|   /");
	asciiArtLines.push_back("         |  \\ /  /");
	asciiArtLines.push_back("          '. '. /");
	asciiArtLines.push_back("            '. '");
	asciiArtLines.push_back("            / \\ \\");
	asciiArtLines.push_back("           / / \\'=,");
	asciiArtLines.push_back("     .----' /   \\ (\\__ ");
	asciiArtLines.push_back("    (((____/     \\ \\  )");
	asciiArtLines.push_back("                  '.\\_)");


	for (size_t i = 0; i < asciiArtLines.size(); ++i) {
		std::string msgAsciiArt = "PRIVMSG " + userNick + " :" + asciiArtLines[i] + "\r\n";
		sendMessage(this->_socketFdBot, msgAsciiArt);
	}
}

void Server::BotJoinChannel(const std::string& channel) {
	std::string msgJoin = "JOIN " + channel + "\r\n";
    sendMessage(this->_socketFdBot, msgJoin);
}

void Server::createBot(const std::string& server, int port) {
    this->_socketFdBot  = socket(AF_INET, SOCK_STREAM, 0);
	struct sockaddr_in server_addr;
	struct hostent *server_host;

	server_host = gethostbyname(server.c_str());
	if (server_host == NULL) {
		std::cout << "Error, no such host" << std::endl;
		return;
	}

	if (this->_socketFdBot  < 0) {
		std::cout << "Error opening socket" << std::endl;
		return;
	}

	memset((char *) &server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = AF_INET;
	memcpy((char *) &server_addr.sin_addr.s_addr, (char *) server_host->h_addr, server_host->h_length);
	server_addr.sin_port = htons(port);

	if (connect(this->_socketFdBot , (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
		std::cout << "Error connecting" << std::endl;
		close(this->_socketFdBot );
		return;
	}

    std::string msgPass = "PASS pass\r\n";
	std::string msgNick = "NICK BOT\r\n";
	std::string msgUser = "USER BOT 0 * :realname\r\n";

    sendMessage(this->_socketFdBot, msgPass);
    sendMessage(this->_socketFdBot, msgNick);
    sendMessage(this->_socketFdBot, msgUser);
}
