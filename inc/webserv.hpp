#pragma once

#include <cerrno>
#include <cstring>
#include <sstream>
#include <iostream> // For cout
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <vector>
#include <map>
#include <set>
#include <iterator>
#include <string>



#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <arpa/inet.h> // For inet_addr("127.0.0.1")
#include <poll.h>
#include <unistd.h> // For read
#include <fcntl.h>// for open
#include <sys/stat.h> // for struct stat
#include <stdio.h> // for perror
#include <signal.h>
#include <fstream> // for getline
#include <dirent.h> //to get directory content

# define BLACK "\x1b[30m"
# define RED "\x1b[31m"
# define GREEN "\x1b[32m"
# define YELLOW "\x1b[33m"
# define BLUE "\x1b[34m"
# define MAGENTA "\x1b[35m"
# define CYAN "\x1b[36m"
# define RESET "\x1b[0m"
# define BgRED "\x1b[41m"
# define BgGREEN "\x1b[42m"
# define BgYELLOW "\x1b[43m"
# define BgBLUE "\x1b[44m"
# define BgMAGENTA "\x1b[45m"
# define BgCYAN "\x1b[46m"
# define BgWHITE "\x1b[47m"

# define PORT 8888
# define BACKLOG 100 // the maximum number of connections that will be queued
# define BUFFER_SIZE 102400

# define DEFAULT_PORT		8080
# define DEFAULT_ERROR_PAGE	"errors/404.html"
# define DEFAULT_INDEX		"index.html"
# define DEFAULT_MAX_BODY	1024 * 1024



template <typename T>
std::string numberToString ( T Number )
{
	std::ostringstream ss;
	ss << Number;
	return ss.str();
}

void		baseError(std::string errorText);
int			stringToNumber (std::string &text);
bool		isPositiveDigit(std::string &s);
void		readSemicolon(std::istream &ifs);
std::string	&cutSemicolon(std::string &line);
int         hexToDec(std::string hex);