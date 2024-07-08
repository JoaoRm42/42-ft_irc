
#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../libs.hpp"

class	Server;

class Client {
  private:
	int socket_fd;
	std::string nick;
	std::string user;
	std::string pass;
	std::vector<std::string> channels;
	size_t	numOfChannels;
	bool isValidData;

  public:
	Client();
	~Client();

	//Getters of Variables
	int getSocketFD() const;
	std::string getNick() const;
	std::string getUser() const;
	std::string getPass() const;
	bool getValidData() const;
	size_t getNumOfChannels() const;

	//Setters of Variables
	void setSocketFD(unsigned int data);
	void setNick(const std::string& data);
	void setUser(const std::string& data);
	void setPass(const std::string& data);
	void setValidData(const bool data);
	int addBackChannel(const std::string& data);

	//Class Functions
	bool checkClientParams(Server &Server, const std::string& buffer);
};


#endif
