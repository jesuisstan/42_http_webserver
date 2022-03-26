#include "Server.hpp"

Server::Server() {
	signal(SIGINT, interruptHandler);
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
		std::cout << "socket() failed" << std::endl;
		exit(-1);
	}
	int optval = 1;
	int ret = setsockopt(this->_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
	if (ret < 0) {
		std::cout << "setsockopt() failed" << std::endl;
		close(this->_listenSocket);
		exit(-1);
	}
	ret = fcntl(this->_listenSocket, F_SETFL, fcntl(_listenSocket, F_GETFL, 0) | O_NONBLOCK);
	if (ret < 0) {
		std::cout << "fcntl() failed" << std::endl;
		close(this->_listenSocket);
		exit(-1);
	}
	this->_servAddr.sin_family = AF_INET;
	this->_servAddr.sin_addr.s_addr = inet_addr(ipAddr);
	this->_servAddr.sin_port = htons(port);
	ret = bind(this->_listenSocket, (struct sockaddr *)&this->_servAddr, sizeof(this->_servAddr));
	if (ret < 0) {
		std::cout << "bind() failed" << std::endl;
		close(this->_listenSocket);
		exit(-1);
	}
	ret = listen(this->_listenSocket, BACKLOG);
	if (ret < 0) {
		std::cout << "listen() failed" << std::endl;
		close(this->_listenSocket);
		exit(-1);
	}
}

void	Server::acceptConnection(void) {
	int newSD = accept(_listenSocket, NULL, NULL);
	if (newSD < 0)
		return ;
	int ret = fcntl(newSD, F_SETFL, fcntl(newSD, F_GETFL, 0) | O_NONBLOCK);
	if (ret < 0) {
		std::cout << "fcntl() failed" << std::endl;
		close(_listenSocket);
		exit(-1);
	}
	std::cout << "New incoming connection:\t" << newSD << std::endl;
	_fds[_numberFds].fd = newSD;
	_fds[_numberFds].events = POLLIN;
	_numberFds++;
}

void	Server::receiveRequest(int socket) {
	std::cout << "Event detected on descriptor:\t" << _fds[socket].fd << std::endl;
	bool closeConnection = false;
	bool compressArray = false;
	int ret = 0;
	char buffer[BUFFER_SIZE] = {0};
	ret = recv(_fds[socket].fd, buffer, BUFFER_SIZE - 1, 0);
	if (ret > 0)
	{
		_clients[socket].reqString += std::string(buffer, ret);
		_clients[socket].reqLength += ret;
		std::cout << _clients[socket].reqLength << " bytes received from sd:\t" << _fds[socket].fd <<  std::endl;
		memset(buffer, 0, BUFFER_SIZE);
		if (findReqEnd(_clients[socket].reqString, _clients[socket].reqLength))
			_fds[socket].events = POLLOUT;
	}
	if (ret == 0 || ret == -1) {
		closeConnection = true;
		if (!ret)
			std::cout << "Request to close connection:\t" << _fds[socket].fd << std::endl;
		else
			std::cout << "recv() failed" << std::endl;
	}
	if (closeConnection) {
		close(_fds[socket].fd);
		std::cout << "Connection has been closed:\t" << _fds[socket].fd << std::endl;
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
				std::cout << "Array of client's descriptors compressed" << std::endl;
			}
		}
	}
}

void	Server::sendResponse(int socket, ServerConfig &config) {
	std::cout << YELLOW << _clients[socket].reqString << RESET << std::endl;
	try {
		_clients[socket].request = RequestParser(_clients[socket].reqString, _clients[socket].reqLength);
		if (_clients[socket].request.getBody().length() > 10000)
			_clients[socket].request.showHeaders();
		else
			std::cout << "|" << YELLOW  << _clients[socket].request.getRequest() << RESET"|" << std::endl;
		_clients[socket].reqString = "";
		_clients[socket].reqLength = 0;
		_clients[socket].response = Response(_clients[socket].request, config);
		char *responseStr = const_cast<char *>(_clients[socket].response.getResponse().c_str());
		std::cout << CYAN << _clients[socket].response.getResponseCode() << RESET << std::endl;
		int ret = send(_fds[socket].fd, responseStr, strlen(responseStr), 0);
		if (ret < 0) {
			std::cout << "send() failed" << std::endl;
			return ;
		}
	}
	catch (RequestParser::UnsupportedMethodException &e) {
		std::cout << e.what() << std::endl;
	}
	_fds[socket].events = POLLIN;
}

void	Server::runServer(int timeout,  ServerConfig &config) {
	_fds[0].fd = _listenSocket;
	_fds[0].events = POLLIN;
	this->setTimeout(timeout);
	this->_numberFds = 1;
	int currentSize = 0;
		std::string requestBuffer = "";
	while (true) {
		std::cout << "Waiting on poll()...\n";
		int ret = poll(_fds, _numberFds, _timeout);
		if (ret < 0) {
			std::cout << "poll() failed" << std::endl;
			break;
		}
		if (ret == 0) {
			std::cout << "poll() timed out. End program.\n";
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
					sendResponse(i, config);
			}
		}
	}
}

void	Server::closeConnections(void) {
	for (int i = 0; i < _numberFds; i++) {
		if (_fds[i].fd >= 0) {
			close(_fds[i].fd);
			std::cout << "Connection successfully closed:\t" << _fds[i].fd << std::endl;
		}
	}
}

bool isChunked(std::string request_buffer) {
    size_t headersEnd = request_buffer.find("\r\n\r\n");
    if (headersEnd != std::string::npos) {
        std::string headers = request_buffer.substr(0, headersEnd);
        size_t transferEncodingHeaderPosition = headers.find("Transfer-Encoding: ");
        if (transferEncodingHeaderPosition != std::string::npos) {
            std::string encoding = headers.substr(transferEncodingHeaderPosition + 19);
            size_t lineEnd = encoding.find("\n");
            if (lineEnd != std::string::npos) {
                encoding = encoding.substr(0, lineEnd - 1);
                if (encoding == "chunked")
                    return true;
            }
        }
    }
    return false;
}

bool Server::findReqEnd(std::string request_buffer, size_t request_len) {
//    std::cout << "|"BLUE  << request_buffer << RESET"|" << std::endl;
    std::string method = request_buffer;
    method = method.substr(0, request_buffer.find_first_of(' '));
//    printStrWithUnprintableChars(request_buffer);
    if (isChunked(request_buffer)) {
        if ((method != "POST" && method != "PUT") || request_buffer.find("0\r\n\r\n") != std::string::npos)
            return true;
        else if ((method != "POST" && method != "PUT") && request_buffer.find("\r\n\r\n") != std::string::npos)
            return true;
    } else if (request_buffer.find("\r\n\r\n") != std::string::npos)
        return true;
    return false;
}

void	interruptHandler(int sig_int) {
	(void)sig_int;
	std::cout << BgMAGENTA << "\nAttention! Interruption signal caught. Web server stopped\n";
	exit (EXIT_SUCCESS);
}
