#include "Server.hpp"

Server::Server() {
	memset(&_servAddr, 0, sizeof(_servAddr));
	memset(_fds, 0, sizeof(_fds));
}

Server::~Server() {
	for (int i = 0; i < _numberFds; i++) {
		if (_fds[i].fd >= 0)
		{
			close(_fds[i].fd);
			pthread_mutex_lock(&g_write);
			std::cerr << "Connection successfully closed:\t" << _fds[i].fd << " (D)" << std::endl;
			pthread_mutex_unlock(&g_write);
		}
	}
}

Server::Server(const Server &other) {
	*this = other;
	return ;
}

Server	&Server::operator = (const Server &other) {
	if (this == &other)
		return (*this);
	this->_listenSocket = other._listenSocket;
	this->_numberFds = other._numberFds;
	this->_servAddr = other._servAddr;
	this->_timeout = other._timeout;
	for (int i = 0; i < _numberFds; i++) {
		this->_fds[i].fd = other._fds[i].fd;
	}
	return (*this);
}

void	Server::setTimeout(int timeout) {
	this->_timeout = timeout;
}

int		Server::getListenSocket(void) const {
	return (this->_listenSocket);
}

int		Server::getTimeout(void) const {
	return (this->_timeout);
}

int		Server::getNumberFds(void) const {
	return (this->_numberFds);
}

void	Server::initiate(const char *ipAddr, int port) {
	this->_listenSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (_listenSocket < 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << "socket() failed" << " on server " << this->serverID << std::endl;
		pthread_mutex_unlock(&g_write);
		exit(-1);
	}
	int optval = 1;
	int ret = setsockopt(this->_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
	if (ret < 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << "setsockopt() failed" << " on server " << this->serverID << std::endl;
		pthread_mutex_unlock(&g_write);
		close(this->_listenSocket);
		exit(-1);
	}
	ret = fcntl(this->_listenSocket, F_SETFL, fcntl(_listenSocket, F_GETFL, 0) | O_NONBLOCK);
	if (ret < 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << "fcntl() failed" << " on server " << this->serverID << std::endl;
		pthread_mutex_unlock(&g_write);
		close(this->_listenSocket);
		exit(-1);
	}
	this->_servAddr.sin_family = AF_INET;
	this->_servAddr.sin_addr.s_addr = inet_addr(ipAddr);
	this->_servAddr.sin_port = htons(port);
	ret = bind(this->_listenSocket, (struct sockaddr *)&this->_servAddr, sizeof(this->_servAddr));
	if (ret < 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << "bind() failed" << " on server " << this->serverID << std::endl;
		pthread_mutex_unlock(&g_write);
		close(this->_listenSocket);
		exit(-1);
	}
	ret = listen(this->_listenSocket, BACKLOG);
	if (ret < 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << "listen() failed" << " on server " << this->serverID << std::endl;
		pthread_mutex_unlock(&g_write);
		close(this->_listenSocket);
		exit(-1);
	}
	return ;
}

void	Server::initReqDataStruct(int clientFD) {
	_clients[_fds[clientFD].fd].reqLength = 0;
	_clients[_fds[clientFD].fd].reqString = "";
	_clients[_fds[clientFD].fd].request = RequestParser();
	_clients[_fds[clientFD].fd].response = Response();
	_clients[_fds[clientFD].fd].isTransfer = false;
	_clients[_fds[clientFD].fd].foundHeaders = false;
	_clients[_fds[clientFD].fd].method = "";
	return ;
}

void	Server::acceptConnection(void) {
	int newSD = accept(_listenSocket, NULL, NULL);
	if (newSD < 0)
		return ;
	int ret = fcntl(newSD, F_SETFL, fcntl(newSD, F_GETFL, 0) | O_NONBLOCK);
	if (ret < 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << "fcntl() failed" << " on server " << this->serverID << std::endl;
		pthread_mutex_unlock(&g_write);
		close(_listenSocket);
		exit(-1);
	}
	pthread_mutex_lock(&g_write);
	std::cerr << "New incoming connection:\t" << newSD << " on server " << this->serverID << std::endl;
	pthread_mutex_unlock(&g_write);
	_fds[_numberFds].fd = newSD;
	_fds[_numberFds].events = POLLIN;
	_numberFds++;
	initReqDataStruct(_fds[_numberFds].fd);
	return ;
}

void	Server::closeConnection(int socket) {
	close(_fds[socket].fd);
	pthread_mutex_lock(&g_write);
	std::cerr << "Connection has been closed:\t" << _fds[socket].fd << " on server " << this->serverID << std::endl;
	pthread_mutex_unlock(&g_write);
	_fds[socket].fd = -1;
	bool compressArray = true;
	if (compressArray) {
		compressArray = false;
		for (int i = 0; i < _numberFds; i++) {
			if (_fds[i].fd == -1)
			{
				for (int j = i; j < _numberFds; j++)
				{
					_fds[j].fd = _fds[j + 1].fd;
				}
				i--;
				_numberFds--;
				pthread_mutex_lock(&g_write);
				if (DEBUG > 1) 
					std::cerr << "Array of client's descriptors compressed" << " on server " << this->serverID << std::endl;
				pthread_mutex_unlock(&g_write);
			}
		}
	}
	return ;
}

void	Server::receiveRequest(int socket) {
	pthread_mutex_lock(&g_write);
	if (DEBUG > 0)
		std::cerr << "Event detected on descriptor:\t" << _fds[socket].fd << " on server " << this->serverID << std::endl;
	pthread_mutex_unlock(&g_write);
	bool closeConnectionFlag = false;
	int ret = 0;
	char buffer[BUFFER_SIZE] = {0};
	ret = recv(_fds[socket].fd, buffer, BUFFER_SIZE - 1, 0);
	if (ret > 0)
	{
		std::string tail = std::string(buffer, ret);
		_clients[socket].reqLength += ret;
		_clients[socket].reqString += tail;
		pthread_mutex_lock(&g_write);
		if (DEBUG > 1)
			std::cerr << _clients[socket].reqLength << " bytes received from sd:\t" << _fds[socket].fd << " on server " << this->serverID <<  std::endl;
		pthread_mutex_unlock(&g_write);
		// memset(buffer, 0, BUFFER_SIZE);
		if (findReqEnd(_clients[socket]))
			_fds[socket].events = POLLOUT;
	}
	if (ret == 0 || ret == -1) {
		closeConnectionFlag = true;
		if (DEBUG > 1) {
			if (!ret) {
				pthread_mutex_lock(&g_write);
				std::cerr << "Request to close connection:\t" << _fds[socket].fd << " on server " << this->serverID << std::endl;
				pthread_mutex_unlock(&g_write);
			}
			else {
				pthread_mutex_lock(&g_write);
				std::cerr << "recv() failed" << " on server " << this->serverID << std::endl;
				pthread_mutex_unlock(&g_write);
			}
		}
	}
	if (closeConnectionFlag)
		closeConnection(socket);
	return ;
}

char	*getCstring(const std::string &cppString) {
	size_t length = cppString.size();
	char *res = (char *)malloc(cppString.size());
	if (!res) {
		std::cerr << "malloc() failed" << std::endl;
		exit(-1);
	}
	for (size_t i = 0; i < length; i++) {
		res[i] = cppString[i];
	}
	return (res);
}

void	Server::sendResponse(int socket) {
	try {
		_clients[socket].request = RequestParser(_clients[socket].reqString, _clients[socket].reqLength);
		if (_clients[socket].request.getBody().length() > 10000)
			_clients[socket].request.showHeaders();
		else
			std::cerr << "|" << YELLOW  << _clients[socket].request.getRequest() << RESET"|" << std::endl;
		_clients[socket].reqString = "";
		_clients[socket].reqLength = 0;
		_clients[socket].foundHeaders = 0;
		_clients[socket].response = Response(_clients[socket].request, webConfig); 
		char *responseStr = getCstring(_clients[socket].response.getResponse());
		size_t responseSize = _clients[socket].response.getResponse().size();
		if (DEBUG > 0)
			std::cerr << CYAN << _clients[socket].response.getResponseCode() << RESET" with size="  << _clients[socket].response.getResponse().size() << std::endl;
		int ret = send(_fds[socket].fd, responseStr, responseSize, 0);
		free (responseStr);
		if (ret < 0) {
			pthread_mutex_lock(&g_write);
			std::cerr << "send() failed" << " on server " << this->serverID << std::endl;
			pthread_mutex_unlock(&g_write);
			closeConnection(socket);
			return ;
		}
	}
	catch (RequestParser::UnsupportedMethodException &e) {
		pthread_mutex_lock(&g_write);
		std::cerr << e.what() << std::endl;
		pthread_mutex_unlock(&g_write);
	}
	_fds[socket].events = POLLIN;
	return ;
}

void	Server::runServer(int timeout) {
	_fds[0].fd = _listenSocket;
	_fds[0].events = POLLIN;
	this->setTimeout(timeout);
	this->_numberFds = 1;
	int currentSize = 0;
		std::string requestBuffer = "";
	while (true) {
		pthread_mutex_lock(&g_write);
		if (DEBUG > 0)
			std::cerr << "Waiting on poll() [server " << this->serverID << "]...\n";
		pthread_mutex_unlock(&g_write);
		int ret = poll(_fds, _numberFds, _timeout);
		if (ret < 0) {
			pthread_mutex_lock(&g_write);
			std::cerr << "poll() failed" << " on server " << this->serverID << std::endl;
			pthread_mutex_unlock(&g_write);
			break;
		}
		if (ret == 0) {
			pthread_mutex_lock(&g_write);
			std::cerr << "poll() timed out. End program." << " on server " << this->serverID << std::endl; ;
			pthread_mutex_unlock(&g_write);
			break;
		}
		currentSize = _numberFds;
		for (int i = 0; i < currentSize; i++) {
			if (_fds[i].revents == 0)
				continue;
			if (_fds[i].revents) {
				if ( _fds[i].fd == _listenSocket  && _fds[i].events == POLLIN)
					acceptConnection();
				if ( _fds[i].fd != _listenSocket  && _fds[i].events == POLLIN)
					receiveRequest(i);
				if (_fds[i].fd != _listenSocket && _fds[i].events == POLLOUT)
					sendResponse(i);
			}
		}
	}
	return ;
}

bool isChunked(std::string headers) {
	size_t startPos = headers.find("Transfer-Encoding:");
	if (startPos != std::string::npos) {
		size_t endLine = headers.find("\n", startPos);
		std::string transferEncodingLine = headers.substr(startPos, endLine - startPos);
		return (transferEncodingLine.find("chunked") != std::string::npos);
	}
	return false;
}

bool Server::endByTimeout(t_reqData &req) {
	// todo add for too long request 
	(void)req;
	return false;
}

bool Server::findReqEnd(t_reqData &req) {
	size_t	pos;
	if (!req.foundHeaders) {
		size_t headersEnd = req.reqString.find("\r\n\r\n");
		if (headersEnd == std::string::npos) // todo заголовок еще не пришел до конца
			return false;
		req.foundHeaders = true;
		req.method = req.reqString.substr(0, req.reqString.find_first_of(' '));
		req.isTransfer = isChunked(req.reqString.substr(0, headersEnd));
	}
	if (!req.isTransfer)
		return true;
	pos = std::max(0, (int)req.reqString.size() - 10);
	if (req.isTransfer and req.reqString.find("0\r\n\r\n", pos) != std::string::npos)
		return true;
	if (req.isTransfer and req.method != "POST" and req.method != "PUT" and req.reqString.find("\r\n\r\n") != std::string::npos)
		return true;
	return false;
}