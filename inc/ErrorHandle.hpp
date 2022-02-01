#ifndef HTTP_WEBSERVER_ERRORHANDLE_HPP
#define HTTP_WEBSERVER_ERRORHANDLE_HPP

#include "webserv.hpp"

int Socket(int domain, int type, int protocol);
void Bind (int sockFd, const struct sockaddr *addr, socklen_t addrLen);
void Listen(int sockFd, int backLog);
int Accept(int sockFd, struct sockaddr *addr, socklen_t *addrLen);
void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
void InetPton(int af, const char* src, void* dst);
char *Recv(int fd, char buffer[1024], size_t bufferSize, int flags);

#endif //HTTP_WEBSERVER_ERRORHANDLE_HPP
