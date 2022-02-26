#pragma once

#include <cerrno>
#include <cstring>
#include <iostream> // For cout
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <vector>

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
# define BUFFER_SIZE 10240
