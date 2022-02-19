NAME			= webserv

SRCS			= main.cpp utils.cpp Server.cpp RequestParser.cpp
SRCS_DIR		= ./src/

INC				= webserv.hpp utils.hpp Server.hpp RequestParser.hpp
INCS_DIR		= ./inc/
INCS			= $(addprefix $(INCS_DIR), $(INC))

OBJ 			= $(SRCS:.cpp=.o)
OBJS_DIR		= ./obj/
OBJS 			= $(addprefix $(OBJS_DIR), $(OBJ))

SRCS_SR			= main.cpp utils.cpp Server.cpp
OBJ_SR 			= $(SRCS_SR:.cpp=.o)
OBJS_SR 		= $(addprefix $(OBJS_DIR), $(OBJ_SR))

SRCS_CL			= client.cpp utils.cpp
OBJ_CL			= $(SRCS_CL:.cpp=.o)
OBJS_CL 		= $(addprefix $(OBJS_DIR), $(OBJ_CL))

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
				$(RM) $(NAME) client server
				@echo "\033[33;1mAll created files were deleted\n\033[0m"

re:				fclean $(NAME)
				@echo "\033[35;1mRemake done\n\033[0m"


srv:			$(OBJS_SR) $(INCS)
				$(CXX) $(FLAGS) -I$(INCS_DIR) $(OBJS_SR) -o server

clt:			$(OBJS_CL) $(INCS)
				$(CXX) $(FLAGS) -I$(INCS_DIR) $(OBJS_CL) -o client