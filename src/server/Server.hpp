#pragma once

#include "webserv.hpp"
#include "Config.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"

extern pthread_mutex_t g_write;
class ServerConfig;
class RequestParser;
class Response;

typedef struct s_reqData {
	std::string		reqString;
	size_t			reqLength;
	RequestParser 	request;
	Response		response;
	bool			isTransfer;
	bool			foundHeaders;
	std::string		method;
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
	class ServerConfig			webConfig;
	int							serverID;
	pthread_t					tid;
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
	void	closeConnection(int socket);
	void	receiveRequest(int socket);
	void	sendResponse(int socket);
	void	runServer(int timeout);
	void	initReqDataStruct(int clientFD);

	// static bool findReqEnd(std::string request_buffer, size_t request_len);
	static bool findReqEnd(t_reqData &req, std::string &tail);
	bool	endByTimeout(t_reqData &req);
};

char	*getCstring(const std::string &cppString);
