//
// Created by joaoped2 on 7/4/24.
//

#ifndef UTILS_HPP
# define UTILS_HPP

# include "../libs.hpp"

std::vector<std::string> split(const std::string &, char);
std::vector<std::string> splitSpace(const std::string &str);
void getArgsPro(std::vector<std::string>* args, std::pair<std::vector<std::string>, std::string> input, size_t start);
size_t findStr(std::string str);
void clearCharPointer(char* str);
std::string returnQuantity(std::string character, int size);
void printInput(std::pair<std::vector<std::string>, std::string> input);
void initInput(std::pair<std::vector<std::string>, std::string>*, std::string);
std::vector<std::string> channelSplit(std::string &line);

#endif
