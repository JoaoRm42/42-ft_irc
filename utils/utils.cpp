//
// Created by joaoped2 on 7/4/24.
//

#include "utills.hpp"

std::vector<std::string> channelSplit(std::string &line) {
	if (line.find('\r') != std::string::npos)
		line.erase(line.find('\r'));
//	std::cout << line << "\n\n\n";
	std::vector<std::string>	result;
	std::string					tmp;
	int i = 0;
	while (line[i]) {
		if (line[i] == ' ') {
			i++;
			result.push_back(tmp);
//			std::cout << tmp << "\n\n\n";
			tmp.clear();
		}
		if (line[i] == ':') {
//			std::cout << "Dois pontos" << "\n\n\n";
			i++;
			while (line[i]) {
				tmp += line[i];
				i++;
			}
			result.push_back(tmp);
			tmp.clear();
		}
		if (line[i] != ' ' || line[i] != ':') {
//			std::cout << line[i] << "\n";
			tmp += line[i];
		}
		if (line[i + 1] == '\0') {
			result.push_back(tmp);
			tmp.clear();
		}
		i++;
	}
	return (result);
}

std::vector<std::string> splitSpace(const std::string &str) {
    // std::cout << std::endl << "split" << std::endl << std::endl;
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (!ss.eof()) {
        // std::cout << "token->$" << token << "$" << std::endl;
        ss >> token;
        tokens.push_back(token);
    }
    // std::cout << std::endl << std::endl;
    return tokens;
}

std::vector<std::string> split(const std::string &str, char delimiter) {
    // std::cout << std::endl << "split" << std::endl << std::endl;
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter)) {
        // std::cout << "token->$" << token << "$" << std::endl;
        tokens.push_back(token);
    }
    // std::cout << std::endl << std::endl;
    return tokens;
}

void getArgsPro(std::vector<std::string>* args, std::pair<std::vector<std::string>, std::string> input, size_t start) {
    size_t i;
    for (i = start; i < input.first.size(); i++)
        args->push_back(input.first[i]);
    if (!input.second.empty())
        args->push_back(input.second);
}

size_t strChecker(std::string str) {
    size_t it = 0, start, end;
    while (str[it] != '\0')
    {
        while (str[it] != '\0' && std::isspace(str[it]) != 0)
            it++;
        start = it;
        while (str[it] != '\0' && std::isspace(str[it]) == 0)
            it++;
        end = it;
        if (end > start)
        {
            std::string tmp = str.substr(start, end - start);
            if (tmp[0] != ':')
                continue ;
            return (start);
        }
        while (str[it] != '\0' && std::isspace(str[it]) != 0)
            it++;
    }
    return (std::string::npos);
}

void    initInput(std::pair<std::vector<std::string>, std::string>* input, std::string line) {
    input->first = splitSpace(line);
    bool message = false;
    size_t len = input->first.size(), i;
    for (i = 0; i < len; i++)
        if (input->first[i].find(":") == 0)
            break ;
    for (; i < len; i++)
    {
        message = true;
        input->first.pop_back();
    }
    if (message == true)
    {
        size_t pos = strChecker(line);
        input->second = line.substr(pos + 1, line.size() - (pos + 1));
    }
}


void clearCharPointer(char* str) {
    if (str) {
        std::memset(str, '\0', std::strlen(str));
    }
}

std::string returnQuantity(std::string character, int size) {
    std::string result;
    while (size-- > 0) {
        result += character;
    }
    return result;
}

void printInput(std::pair<std::vector<std::string>, std::string> input)
{
    std::vector<std::string> saveResult;
    int size = 0;
    for (size_t i = 0; i < input.first.size(); i++) {
        saveResult.push_back("\"");
        size += 1;
        saveResult.push_back(input.first[i]);
        size += input.first[i].size();
        saveResult.push_back("\" ");
        size += 2;
    }

    int totalSize = size + 7 + input.second.size(); // Additional space for "Input", "TEXT", and quotes
    int paddingSize = 20; // Adjust this to ensure the minimum width for "TEXT" line

    if (totalSize < paddingSize) {
        totalSize = paddingSize;
    }

    int halfPadding = (totalSize - size - 5) / 2; // 5 for the length of "Input"

    std::cout << "┌" << returnQuantity("─", halfPadding) << "Input" << returnQuantity("─", totalSize - halfPadding - 5) << "┐" << std::endl;
    std::cout << "│ ARGS: ";
    for (std::vector<std::string>::iterator it = saveResult.begin(); it != saveResult.end(); it++)
        std::cout << *it;
    std::cout << std::setw(totalSize - size - 4) << "│" << std::endl;
    std::cout << "│ TEXT: \"" << input.second << "\"" << std::setw(totalSize - input.second.size() - 6) << "│" << std::endl;
    std::cout << "└" << returnQuantity("─", totalSize) << "┘" << std::endl;
    std::cout << std::endl;
}