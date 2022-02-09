#pragma once
# include "webserv.hpp"

class Server
{
private:
	bool			_serverOnAir = true;
	int 			_listenSocket;
	struct pollfd	_fds[AMMOUNT_FDS];
	int				_timeout;
	int				_numberFds;
	Server();

public:
	Server(int domain, int type, int protocol);
	~Server();
	Server(const Server &other);
	Server	&operator = (const Server &other);
	void	setServerOnAir(bool status);
	void	setTimeout(bool status);
	int		getListenSocket(void) const;
	int		getTimeout(void) const;

};

void	interruptHandler(int sig_int);