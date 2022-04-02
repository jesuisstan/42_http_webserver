#pragma once

#include "webserv.hpp"
#include "Config.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"
# include "../logger/Logger.hpp"

extern pthread_mutex_t g_write;
class ServerConfig;
class RequestParser;
class Response;
class Logger;

typedef struct s_reqData {
	std::string		reqString;
	size_t			reqLength;
	char			*responseStr;
	size_t			responseSize;
	bool			isTransfer;
	bool			foundHeaders;
	std::string		method;
	size_t			chunkInd;
	size_t			lastTime;
	Response		*response;
}	t_reqData;

class Server {
private:
	int 						_listenSocket;
	sockaddr_in					_servAddr;
	std::vector<struct pollfd>	_fds;
	int							_timeout;
	std::map<long, t_reqData>	_clients;
	std::set<int>				_fdToDel;
	std::stringstream 			_message;

	bool	isChunked(std::string headers);
	bool	findReqEnd(t_reqData &req);
	void	pollError(int i);
	void	clearConnections();

	Server(const Server &other);
	Server	&operator = (const Server &other);
public:
	class ServerConfig			webConfig;
	int							serverID;
	pthread_t					tid;
	Server();
	~Server();
	
	void	setTimeout(int timeout);
	int		getListenSocket(void) const;
	int		getTimeout(void) const;
	// int		getNumberFds(void) const;
	void	initiate(const char *ipAddr, int port);
	void	initiate();
	void	acceptConnection(void);
	void	closeConnection(int socket);
	void	receiveRequest(int i);
	void	sendResponse(int i);
	void	runServer(int timeout);
	void	initReqDataStruct(int clientFD);
	void	endByTimeout(void);

	// static bool findReqEnd(std::string request_buffer, size_t request_len);
	
	bool	endByTimeout(t_reqData &req);
};


char	*getCstring(const std::string &cppString);