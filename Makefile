NAME			= webserv

CXX				= clang++

FLAGS			= -MMD -Wall -g -pthread
FLAGS			= -MMD -Wall -Wextra -Werror -g -pthread -std=c++98

RM				= rm -rf

INCLUDES		= -I inc

CLASS_HDRS		= -I src/server -I src/config -I src/request -I src/response -I src/cgi -I src/logger

HEADERS			= webserv.hpp

CONFIG			= Config	ServerConfig	Location	utils
SERVER			= Server
REQUEST			= RequestParser
RESPONSE		= Response
CGI				= Cgi
LOGGER			= Logger

SRC_DIR 		= src
SRC				= $(addsuffix .cpp, main) \
				$(addprefix config/, $(addsuffix .cpp, $(CONFIG))) \
				$(addprefix server/, $(addsuffix .cpp, $(SERVER))) \
				$(addprefix request/, $(addsuffix .cpp, $(REQUEST))) \
				$(addprefix response/, $(addsuffix .cpp, $(RESPONSE))) \
				$(addprefix cgi/, $(addsuffix .cpp, $(CGI))) \
				$(addprefix logger/, $(addsuffix .cpp, $(LOGGER)))

OBJ_DIR			= obj
OBJ				= $(addprefix $(OBJ_DIR)/, $(addsuffix .o, main)) \
				$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(CONFIG))) \
				$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(SERVER))) \
				$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(REQUEST))) \
				$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(RESPONSE))) \
				$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(CGI))) \
				$(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(LOGGER)))

OBJ_SUBDIR		= $(addprefix $(OBJ_DIR)/, config server request response cgi logger)

OBJ_BUILD		= $(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))
MMD_FILES		= $(OBJ_BUILD:.o=.d)

.PHONY:			all clean fclean re

all:			$(NAME)

$(NAME):		$(OBJ_DIR) $(OBJ_BUILD)
				$(CXX) $(FLAGS) $(INCLUDES) $(CLASS_HDRS) -o $(NAME) $(OBJ_BUILD)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
				$(CXX) $(FLAGS) $(INCLUDES) $(CLASS_HDRS) -o $@ -c $<

$(OBJ_DIR):
				mkdir -p $(OBJ_DIR)
				mkdir -p $(OBJ_SUBDIR)

clean:
				$(RM) $(OBJ_DIR) $(OBJ_SUBDIR)
				@echo "\033[32;1mCleaning succeed\n\033[0m"

fclean:			clean
				$(RM) $(NAME)
				$(RM) client
				$(RM) ./www/put_image/*
				@echo "\033[33;1mAll created files were deleted\n\033[0m"

re:				fclean $(NAME)
				@echo "\033[35;1mRemake done\n\033[0m"

-include $(MMD_FILES)