#pragma once

# include "webserv.hpp"

class RequestParser;

class Server {
private:
	int 			_listenSocket;
	sockaddr_in		_servAddr;
	struct pollfd	_fds[BACKLOG];
	int				_timeout;
	int				_numberFds;

public:
	Server();
	~Server();
	Server(const Server &other);
	Server	&operator = (const Server &other);
	void	setTimeout(int timeout);
	int		getListenSocket(void) const;
	int		getTimeout(void) const;
	int		getNumberFds(void) const;
	void	initiate(const char *ipAddr, int port);
	void	runServer(int timeout);
	void	closeConnections(void);
};

void	interruptHandler(int sig_int);
char 	*createResponse(char *buffer, const char *file, RequestParser parser);