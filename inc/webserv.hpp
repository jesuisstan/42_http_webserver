#pragma once

#include <iostream> // For cout
#include <sys/socket.h> // For socket functions
#include <netinet/in.h> // For sockaddr_in
#include <unistd.h> // For read
#include <cstdlib> // For exit() and EXIT_FAILURE
#include <arpa/inet.h> // For inet_addr("127.0.0.1")

# define RESET "\033[0m" // set color to default
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"

# define BACKLOG 7 // the maximum number of connections that will be queued
# define PORT 8888
# define BUFFER_SIZE 1024