# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: marvin <marvin@student.42.fr>              +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/06/17 11:06:08 by marvin            #+#    #+#              #
#    Updated: 2024/06/17 11:06:08 by marvin           ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = ircserv
CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -g
SRCS = main.cpp server/server.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME):	$(OBJS)
			@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

clean:
			@rm -rf $(OBJS)

fclean:		clean
			@rm -rf $(NAME)

re:			fclean all