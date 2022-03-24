#pragma once

#include "../config/Config.hpp"
#include "../../inc/webserv.hpp"

class RequestParser;
class ServerConfig;

typedef struct s_reqData {
	std::string	reqString;
	size_t		regLength;
}	t_reqData;

class Server {
private:
	int 						_listenSocket;
	sockaddr_in					_servAddr;
	struct pollfd				_fds[BACKLOG];
	int							_timeout;
	int							_numberFds;
	std::map<long, t_reqData>	_reqBuffer;

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
	void	acceptConnection(void);
	void	receiveRequest(int socket);
	void	handleRequest(int socket, ServerConfig &config);
	void	runServer(int timeout, ServerConfig &config);
	void	closeConnections(void);

	static bool findReqEnd(std::string requestBuffer, size_t requestLength);
};

void	interruptHandler(int sig_int);
