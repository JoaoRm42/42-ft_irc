//
// Created by joaoped2 on 7/4/24.
//

#include "utils.hpp"

std::string toUpperCase(const std::string& input) {
	std::string result = input;

	for (size_t i = 0; i < result.length(); ++i) {
		result[i] = std::toupper(result[i]);
	}

	return result;
}

std::vector<std::string> returnVector(std::vector<std::string> vector) {
	std::vector<std::string> tmp;
	for (std::vector<std::string>::iterator it = vector.begin(); it != vector.end(); it++) {
		tmp.push_back(*it);
	}
	return (tmp);
}

void	printVector(std::vector<std::string> vector) {
	int	i = 0;
	for (std::vector<std::string>::iterator it = vector.begin(); it != vector.end(); it++) {
		std::cout << "Vector string[" << i << "]: " << *it << std::endl;
		i++;
	}
	std::cout << std::endl;
	return ;
}

std::vector<std::string> channelSplit(std::string &line) {
	size_t pos = line.find('\r');
	if (pos != std::string::npos)
		line.erase(pos);

	std::vector<std::string> result;
	std::string tmp;
	size_t i = 0;  // Use size_t for the index to avoid signed/unsigned comparison issues

	// Loop through the string within bounds
	while (i < line.length()) {
		if (line[i] == ' ') {
			if (!tmp.empty()) {
				result.push_back(tmp);
				tmp.clear();
			}
			i++;
			continue;  // Skip the space
		}

		if (line[i] == ':') {
			if (!tmp.empty()) {
				result.push_back(tmp);
				tmp.clear();
			}
			i++;
			// Append the rest of the string after the colon
			while (i < line.length()) {
				tmp += line[i];
				i++;
			}
			if (!tmp.empty()) {
				result.push_back(tmp);
				tmp.clear();
			}
			break;  // End the loop since we are done
		}

		// Append characters that are not spaces or colons
		tmp += line[i];
		i++;

		// Push the last accumulated string if it's not empty
		if (i == line.length() && !tmp.empty()) {
			result.push_back(tmp);
			tmp.clear();
		}
	}

	return result;
}

std::vector<std::string> splitSpace(const std::string &str) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (!ss.eof()) {
        ss >> token;
        tokens.push_back(token);
    }
    return tokens;
}

std::vector<std::string> split(const std::string &str, char delimiter) {
    std::vector<std::string> tokens;
    std::stringstream ss(str);
    std::string token;
    while (std::getline(ss, token, delimiter))
        tokens.push_back(token);
    return tokens;
}

void getArgsPro(std::vector<std::string>* args, std::pair<std::vector<std::string>, std::string> input, size_t start) {
    size_t i;
    for (i = start; i < input.first.size(); i++)
        args->push_back(input.first[i]);
    if (!input.second.empty())
        args->push_back(input.second);
}

size_t checkStr(const std::string& str) {
    size_t it;
    size_t start;
    size_t end;
    std::string tmp;

    it = 0;
    start = 0;
    end = 0;
    while (it < str.size()) {
        while (it < str.size() && std::isspace(str[it]))
            it++;

        start = it;

        while (it < str.size() && !std::isspace(str[it]))
            it++;

        end = it;

        if (end > start) {
            tmp = str.substr(start, end - start);
            if (!tmp.empty() && tmp[0] == ':')
                return (start);
        }
    }

    return (std::string::npos);
}

void    initInput(std::pair<std::vector<std::string>, std::string>* input, const std::string &line) {
    bool message;
    size_t pos;
    size_t	len;
    size_t i;

    message = false;
    pos = 0;
    len = 0;
    i = 0;
    input->first = splitSpace(line);
    len = input->first.size();
    for (i = 0; i < len; i++) {
        if (input->first[i].find(":") == 0) {
            if (input->first[i] == ":realname")
                ;
            else
                break;
        }
    }
    while (i < len) {
        message = true;
        input->first.pop_back();
        i++;
    }
    if (message) {
        pos = checkStr(line);
        input->second = line.substr(pos + 1, line.size() - (pos + 1));
    }
}


void clearCharPointer(char* str) {
    if (str) {
        std::memset(str, '\0', std::strlen(str));
    }
}

std::string returnQuantity(const std::string &character, unsigned int size) {
    std::string result;

    while (size-- > 0) {
        result += character;
    }

    return (result);
}

void printInput(std::pair<std::vector<std::string>, std::string> input, Client* client) {
    std::vector<std::string>	saveResult;
    unsigned int				size;
    unsigned int				totalSize;
    unsigned int				paddingSize;
    unsigned int				halfPadding;

    size = 0;
    totalSize = 0;
    paddingSize = 0;
    halfPadding = 0;
    for (size_t i = 0; i < input.first.size(); i++) {
        saveResult.push_back("\"");
        size += 1;
        saveResult.push_back(input.first[i]);
        size += input.first[i].size();
        saveResult.push_back("\" ");
        size += 2;
    }

    totalSize = size + 7 + input.second.size(); // Additional space for "Input", "TEXT", and quotes
    paddingSize = 20; // Adjust this to ensure the minimum width for "TEXT" line

    if (totalSize < paddingSize) {
        totalSize = paddingSize;
    }

    halfPadding = (totalSize - size - 5) / 2; // 5 for the length of "Input"

    std::cout << "┌" << returnQuantity("─", halfPadding) << "Input" << returnQuantity("─", totalSize - halfPadding - 5) << "┐" << std::endl;
    if (!client->getNick().empty())
        std::cout << "│ User: " << client->getNick() << std::setw(totalSize - 4 - client->getNick().size()) << "│" << std::endl;
    std::cout << "│ ARGS: ";
    for (std::vector<std::string>::iterator it = saveResult.begin(); it != saveResult.end(); it++)
        std::cout << *it;
    std::cout << std::setw(totalSize - size - 4) << "│" << std::endl;
    std::cout << "│ TEXT: \"" << input.second << "\"" << std::setw(totalSize - input.second.size() - 6) << "│" << std::endl;
    std::cout << "└" << returnQuantity("─", totalSize) << "┘" << std::endl;
    std::cout << std::endl;
}