//
// Created by joaoped2 on 7/4/24.
//

#ifndef UTILS_HPP
# define UTILS_HPP

# include "../libs.hpp"

std::string					toUpperCase(const std::string& input);
std::vector<std::string>	split(const std::string &, char);
std::vector<std::string>	splitSpace(const std::string &str);
void						getArgsPro(std::vector<std::string>* args, std::pair<std::vector<std::string>, std::string> input, size_t start);
size_t						checkStr(const std::string& str);
std::vector<std::string>	channelSplit(std::string &line);
void						clearCharPointer(char *str);
std::string					returnQuantity(const std::string &character, int size);
void						printInput(std::pair<std::vector<std::string>, std::string> input, Client* client);
void						initInput(std::pair<std::vector<std::string>, std::string>* input, const std::string &line);
void						printVector(std::vector<std::string> vector);
std::vector<std::string>	returnVector(std::vector<std::string> vector);

#endif
