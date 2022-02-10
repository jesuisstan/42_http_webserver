#pragma once

# include "webserv.hpp"

class Server
{
private:
	std::string		_serverName;
	bool			_serverOnAir;
	int 			_listenSocket;
	sockaddr_in		_servAddr;
	struct pollfd	_fds[AMMOUNT_FDS];
	int				_timeout;
	int				_numberFds;
	Server();

public:
	Server(const std::string &name);
	//Server(const char *ipAddr, int port);
	~Server();
	Server(const Server &other);
	Server	&operator = (const Server &other);
	void	setServerOnAir(bool status);
	void	setTimeout(int timeout);
	int		getListenSocket(void) const;
	int		getTimeout(void) const;
	//void	interruptHandler(int sig_int);
	void	initiate(const char *ipAddr, int port);
	void	runServer(int timeout);
	void	closeConnections(void);

};

char 	*createResponse(char *buffer, const char *file);
//void	interruptHandler(int sig_int);