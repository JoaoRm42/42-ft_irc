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

FRAME1 := ⠁⠂⠄⡀⠄⠂⠁
FRAME2 := ⠂⠄⡀⠄⠂⠁⠂
FRAME3 := ⠄⡀⠄⠂⠁⠂⠄
FRAME4 := ⡀⠄⠂⠁⠂⠄⡀
FRAME5 := ⠄⠂⠁⠂⠄⡀⠄
FRAME6 := ⠂⠁⠂⠄⡀⠄⠂

FRAMES := $(FRAME1) $(FRAME2) $(FRAME3) $(FRAME4) $(FRAME5) $(FRAME6)


NAME = ircserv
CXX = c++
CXXFLAGS = -std=c++98 -Wall -Wextra -Werror -g
SRCS = main.cpp \
	server/server.cpp \
	server/getHostname.cpp \
	server/getChannel.cpp \
	channel/Channel.cpp \
	server/channelOperators/joinCommand.cpp \
	server/channelOperators/partCommand.cpp \
	utils/utils.cpp \
	server/channelOperators/kickCommand.cpp
OBJS = $(SRCS:.cpp=.o)
OBJ_DIR = obj
OBJ_FILES = $(addprefix $(OBJ_DIR)/, $(OBJS))

all: $(NAME)

$(NAME): $(OBJ_FILES)
			@echo "Done!"
			@sleep 0.5
			@clear
			for frame in $(FRAMES); do \
				clear; \
				echo "$$frame Linking..."; \
				sleep 0.2; \
			done;
			@clear
			@$(CXX) $(CXXFLAGS) $(OBJ_FILES) -o $(NAME)
			@echo "Compilation Done!"
			@sleep 0.5

$(OBJ_DIR)/%.o: %.cpp
			@mkdir -p $(dir $@)
			for frame in $(FRAMES); do \
				clear; \
				echo "$$frame Compiling $<"; \
				sleep 0.1; \
			done;
			@clear
			@echo "$(FRAME6) Compiled $<..."
			@$(CXX) $(CXXFLAGS) -c $< -o $@
			@sleep 0.5
			@clear


clean:
			@clear
			for frame in $(FRAMES); do \
				clear; \
				echo "$$frame Cleaning..."; \
				sleep 0.2; \
			done;
			@clear
			@rm -rf $(OBJ_DIR)
			@echo "Done!"

fclean: clean
			@clear
			for frame in $(FRAMES); do \
				clear; \
				echo "$$frame Force cleaning..."; \
				sleep 0.2; \
			done;
			@clear
			@rm -rf $(NAME)
			@echo "Done!"

re: fclean all
			@clear
			@echo "Rebuild complete!"