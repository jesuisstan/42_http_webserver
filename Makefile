# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: St.Krivtsov <1987stanislav@gmail.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2021/12/28 19:12:31 by St.Krivtsov       #+#    #+#              #
#    Updated: 2022/01/28 22:33:19 by St.Krivtsov      ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= webserv

SRCS			= server.cpp utils.cpp
SRCS_DIR		= ./src/

INC				= webserv.hpp
INCS_DIR		= ./inc/
INCS			= $(addprefix $(INCS_DIR), $(INC))

OBJ 			= $(SRCS:.cpp=.o)
OBJS_DIR		= ./obj/
OBJS 			= $(addprefix $(OBJS_DIR), $(OBJ))

CXX				= c++

FLAGS			= -Wall -Wextra -Werror -std=c++98

RM				= rm -rf

.PHONY:			all clean fclean re

all:			$(NAME)

$(OBJS_DIR)%.o:	$(SRCS_DIR)%.cpp $(INCS)
				@mkdir -p $(OBJS_DIR)
				$(CXX) $(FLAGS) -I$(INCS_DIR) -c -g $< -o $@

$(NAME):		$(OBJS) $(INCS)
				$(CXX) $(FLAGS) -I$(INCS_DIR) $(OBJS) -o $(NAME)

clean:
				$(RM) $(OBJS) $(OBJS_DIR)
				@echo "\033[32;1mCleaning succeed\n\033[0m"

fclean:			clean
				$(RM) $(NAME)
				@echo "\033[33;1mAll created files were deleted\n\033[0m"

re:				fclean $(NAME)
				@echo "\033[35;1mRemake done\n\033[0m"
