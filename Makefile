# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: joaoped2 <joaoped2@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/17 11:06:08 by marvin            #+#    #+#              #
#    Updated: 2024/06/18 11:25:01 by joaoped2         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
#CXX = c++
#CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -g
#-------------------FOR CLION-------------------
CXX = g++
CXXFLAGS = -std=c++98 -g

SRCS = main.cpp \
	server/server.cpp \
	server/getHostname.cpp \
	server/getChannel.cpp \
	channel/Channel.cpp \
	client/Client.cpp \
	server/channelOperators/joinCommand.cpp \
	server/channelOperators/partCommand.cpp \
	utils/utils.cpp \
	server/channelOperators/kickCommand.cpp \
	server/channelOperators/modeCommand.cpp \
	server/channelOperators/topicCommand.cpp \
	server/channelOperators/inviteCommand.cpp \
	Bot/Bot.cpp \
	server/channelOperators/quitCommand.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME):	$(OBJS)
			@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
			@rm -rf $(OBJS)

fclean:		clean
			@rm -rf $(NAME)

re:			fclean all
