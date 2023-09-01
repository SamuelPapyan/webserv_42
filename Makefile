# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: spapyan <spapyan@student.42.fr>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/09/01 12:06:58 by spapyan           #+#    #+#              #
#    Updated: 2023/09/01 12:06:58 by spapyan          ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

SRCS = srcs/main.cpp \
		srcs/Utils.cpp \
		srcs/ServerManager.cpp \
		srcs/Response.cpp \
		srcs/Client.cpp \
		srcs/HttpRequest.cpp \
		srcs/ConfigFile.cpp \
		srcs/ConfigParse.cpp \
		srcs/ServerManager.cpp \
		srcs/Location.cpp \
		srcs/CgiHandler.cpp \
		srcs/Mime.cpp \
		srcs/Logger.cpp

HEADERS = includes/Webserv.hpp

OBJS = $(SRCS:.cpp=.o)

CC = c++

CFLAGS = -Wall -Werror -Wextra -std=c++98

RM = rm -rf

RESET = "\033[0m"
BLACK = ="\033[1m\033[37m"

all:
	@$(MAKE) $(NAME) -j5
$(NAME): $(OBJS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)
	@echo $(BLACK) - webserv compiled $(RESET)

clean:
	$(RM) $(OBJS)

fclean: clean
	$(RM) $(NAME)

re:	fclean all

.PHONY: all clean fclean re
