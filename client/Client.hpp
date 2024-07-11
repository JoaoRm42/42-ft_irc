
#ifndef CLIENT_HPP
# define CLIENT_HPP

#include "../libs.hpp"

class	Server;

class Client {
  private:
	int socket_fd;
	std::string nick;
	std::string user;
	std::vector<std::string> channels;
	size_t	numOfChannels;
	bool isValidData;
	bool isValidNick;
	bool validatedPass;
	bool validatedUser;

  public:
	Client();
	~Client();

	//Getters of Variables
	int getSocketFD() const;
	std::string getNick() const;
	std::string getUser() const;
	bool getValidData() const;
	size_t getNumOfChannels() const;
	std::vector<std::string> getChannels(void) const;

	//Setters of Variables
	void setSocketFD(unsigned int data);
	void setNick(const std::string& data);
	void setUser(const std::string& data);
	void setPass(const std::string& data);
	void setValidData(const bool data);
	int addBackChannel(const std::string& data);

	//Class Functions
	bool checkClientParams(Server &Server, const std::string& buffer);
	bool checkForbiddenChars(const std::string& toCheck);
	void checkNick(Server &Server, const std::vector<std::string> &tmp);
	void checkUser(Server &Server, const std::vector<std::string> &tmp);
};


#endif
