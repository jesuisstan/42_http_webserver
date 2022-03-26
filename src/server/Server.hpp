#pragma once

#include "webserv.hpp"
#include "Config.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"

class ServerConfig;
class RequestParser;
class Response;

typedef struct s_reqData {
	std::string		reqString;
	size_t			reqLength;
	RequestParser 	request;
	Response		response;
}	t_reqData;

class Server {
private:
	int 						_listenSocket;
	sockaddr_in					_servAddr;
	struct pollfd				_fds[BACKLOG];
	int							_timeout;
	int							_numberFds;
	std::map<long, t_reqData>	_clients;

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
	void	receiveRequest(int socket, ServerConfig &config);
	void	sendResponse(int socket, ServerConfig &config);
	void	runServer(int timeout, ServerConfig &config);
	void	closeConnections(void);

	static bool findReqEnd(std::string request_buffer, size_t request_len);
};

void	interruptHandler(int sig_int);
