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
			std::cout << "Connection successfully closed:\t" << _fds[i].fd << " (D)" << std::endl;
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
		std::cout << "socket() failed" << " on server " << this->serverID << std::endl;
		exit(-1);
	}
	int optval = 1;
	int ret = setsockopt(this->_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
	if (ret < 0) {
		std::cout << "setsockopt() failed" << " on server " << this->serverID << std::endl;
		close(this->_listenSocket);
		exit(-1);
	}
	ret = fcntl(this->_listenSocket, F_SETFL, fcntl(_listenSocket, F_GETFL, 0) | O_NONBLOCK);
	if (ret < 0) {
		std::cout << "fcntl() failed" << " on server " << this->serverID << std::endl;
		close(this->_listenSocket);
		exit(-1);
	}
	this->_servAddr.sin_family = AF_INET;
	this->_servAddr.sin_addr.s_addr = inet_addr(ipAddr);
	this->_servAddr.sin_port = htons(port);
	ret = bind(this->_listenSocket, (struct sockaddr *)&this->_servAddr, sizeof(this->_servAddr));
	if (ret < 0) {
		std::cout << "bind() failed" << " on server " << this->serverID << std::endl;
		close(this->_listenSocket);
		exit(-1);
	}
	ret = listen(this->_listenSocket, BACKLOG);
	if (ret < 0) {
		std::cout << "listen() failed" << " on server " << this->serverID << std::endl;
		close(this->_listenSocket);
		exit(-1);
	}
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
		std::cout << "fcntl() failed" << " on server " << this->serverID << std::endl;
		close(_listenSocket);
		exit(-1);
	}
	std::cout << "New incoming connection:\t" << newSD << " on server " << this->serverID << std::endl;
	_fds[_numberFds].fd = newSD;
	_fds[_numberFds].events = POLLIN;
	_numberFds++;
	initReqDataStruct(_fds[_numberFds].fd);
}

void	Server::receiveRequest(int socket) {
	std::cout << "Event detected on descriptor:\t" << _fds[socket].fd << " on server " << this->serverID << std::endl;
	bool closeConnection = false;
	bool compressArray = false;
	int ret = 0;
	char buffer[BUFFER_SIZE] = {0};
	ret = recv(_fds[socket].fd, buffer, BUFFER_SIZE - 1, 0);
	if (ret > 0)
	{
		std::string tail = std::string(buffer, ret);
		_clients[socket].reqLength += ret;
		_clients[socket].reqString += tail;
		std::cout << _clients[socket].reqLength << " bytes received from sd:\t" << _fds[socket].fd << " on server " << this->serverID <<  std::endl;
		memset(buffer, 0, BUFFER_SIZE);
		if (findReqEnd(_clients[socket], tail))
			_fds[socket].events = POLLOUT;
	}
	if (ret == 0 || ret == -1) {
		closeConnection = true;
		if (!ret)
			std::cout << "Request to close connection:\t" << _fds[socket].fd << " on server " << this->serverID << std::endl;
		else
			std::cout << "recv() failed" << " on server " << this->serverID << std::endl;
	}
	if (closeConnection) {
		close(_fds[socket].fd);
		std::cout << "Connection has been closed:\t" << _fds[socket].fd << " on server " << this->serverID << std::endl;
		_fds[socket].fd = -1;
		compressArray = true;
	}
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
				std::cout << "Array of client's descriptors compressed" << " on server " << this->serverID << std::endl;
			}
		}
	}
}

char	*getCstring(const std::string &cppString) {
	size_t length = cppString.size();
	char *res = (char *)malloc(cppString.size());
	if (!res) {
		std::cout << "malloc() failed" << std::endl;
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
			std::cout << "|" << YELLOW  << _clients[socket].request.getRequest() << RESET"|" << std::endl;
		_clients[socket].reqString = "";
		_clients[socket].reqLength = 0;
		_clients[socket].foundHeaders = 0;
		_clients[socket].response = Response(_clients[socket].request, webConfig); 
		char *responseStr = getCstring(_clients[socket].response.getResponse());
		size_t responseSize = _clients[socket].response.getResponse().size();
		std::cout << CYAN << _clients[socket].response.getResponseCode() << RESET << std::endl;
		int ret = send(_fds[socket].fd, responseStr, responseSize, 0);
		free (responseStr);
		if (ret < 0) {
			std::cout << "send() failed" << " on server " << this->serverID << std::endl;
			return ;
		}
	}
	catch (RequestParser::UnsupportedMethodException &e) {
		std::cout << e.what() << std::endl;
	}
	_fds[socket].events = POLLIN;
}

void	Server::runServer(int timeout) {
	_fds[0].fd = _listenSocket;
	_fds[0].events = POLLIN;
	this->setTimeout(timeout);
	this->_numberFds = 1;
	int currentSize = 0;
		std::string requestBuffer = "";
	while (true) {
		std::cout << "Waiting on poll() [server " << this->serverID << "]...\n";
		int ret = poll(_fds, _numberFds, _timeout);
		if (ret < 0) {
			std::cout << "poll() failed" << " on server " << this->serverID << std::endl;
			break;
		}
		if (ret == 0) {
			std::cout << "poll() timed out. End program." << " on server " << this->serverID << std::endl; ;
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
}

void	Server::closeConnections(void) {
	for (int i = 0; i < _numberFds; i++) {
		if (_fds[i].fd >= 0) {
			close(_fds[i].fd);
			std::cout << "Connection successfully closed:\t" << _fds[i].fd << " on server " << this->serverID << std::endl;
		}
	}
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
	return false;
}

bool Server::findReqEnd(t_reqData &req, std::string &tail) {
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
	if (req.isTransfer and req.reqString.find("0\r\n\r\n") != std::string::npos)
		return true;
	if (req.isTransfer and req.method != "POST" and req.method != "PUT" and req.reqString.find("\r\n\r\n") != std::string::npos)
		return true;
	return false;
}
