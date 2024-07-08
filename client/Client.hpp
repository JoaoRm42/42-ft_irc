
#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../libs.hpp"

class Client {
  private:
	int socket_fd;
	std::string nick;
	std::string user;
	std::string pass;
	std::vector<std::string> channels;
	size_t	numOfChannels;

  public:
	Client();
	~Client();

	//Getters of Variables
	int getSocketFD() const;
	std::string getNick() const;
	std::string getUser() const;
	std::string getPass() const;
	size_t getNumOfChannels() const;

	//Setters of Variables
	void setSocketFD(unsigned int data);
	void setNick(std::string data);
	void setUser(std::string data);
	void setPass(std::string data);
	int addBackChannel(std::string data);

	//Class Functions
	bool checkClientParams(std::string password, char *buffer);
};


#endif
