#include "webserv.hpp"
#include "Server.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"

Server::Server() {
	memset(&_servAddr, 0, sizeof(_servAddr));
	memset(_fds, 0, sizeof(_fds));
	signal(SIGINT, interruptHandler);
}

Server::~Server() {
	for (int i = 0; i < _numberFds; i++)
	{
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
	if (newSD < 0) {
		std::cout << "accept() failed. Unable to add a new client" << std::endl;
		return ;
	}
	int ret = fcntl(newSD, F_SETFL, fcntl(newSD, F_GETFL, 0) | O_NONBLOCK);
	if (ret < 0) {
		std::cout << "fcntl() failed" << std::endl;
		close(_listenSocket);
		exit(-1);
	}
	std::cout << "New incoming connection:\t" << newSD << std::endl;
	_fds[_numberFds].fd = newSD;
	_fds[_numberFds].events = POLLIN;
	t_reqData newReqData;
	_reqBuffer.insert(std::make_pair(_fds[_numberFds].fd, newReqData));
	_numberFds++;
}

void	Server::receiveRequest(int socket) {
	std::cout << "Event detected on descriptor:\t" << _fds[socket].fd << std::endl;
	bool closeConnection = false;
	bool compressArray = false;
	while (!closeConnection) {
		char buffer[BUFFER_SIZE] = {0};
		int ret = recv(_fds[socket].fd, buffer, BUFFER_SIZE - 1, 0);
		if (ret <= 0) {
			if (ret == 0)
				closeConnection = true;
			break;
		}
		_reqBuffer[socket].reqString += static_cast<std::string>(buffer).substr(0, ret);
		_reqBuffer[socket].regLength += ret;
		std::cout << _reqBuffer[socket].regLength << " bytes received from sd:\t" << _fds[socket].fd <<  std::endl;
		memset(buffer, 0, BUFFER_SIZE);
	}
	if (findReqEnd(_reqBuffer[socket].reqString, _reqBuffer[socket].regLength))
		_fds[socket].events = POLLOUT;
	if (closeConnection) {
		std::cout << "Connection has been closed:\t" << _fds[socket].fd << std::endl;
		close(_fds[socket].fd);
		_reqBuffer[socket].reqString = "";
		_reqBuffer[socket].regLength = 0;
		_fds[socket].fd = -1;
		compressArray = true;
	}
	if (compressArray) {
		compressArray = false;
		for (int i = 0; i < _numberFds; i++) {
			if (_fds[i].fd == -1)
			{
				for (int j = i; j < _numberFds; j++) {
					_fds[j].fd = _fds[j + 1].fd;
				}
				i--;
				_numberFds--;
				std::cout << "Array of client's descriptors compressed" << std::endl;
			}
		}
	}
}

void	Server::handleRequest(int socket, ServerConfig &config) {
	std::cout << YELLOW << _reqBuffer[socket].reqString << RESET << std::endl;
	try {
		RequestParser request = RequestParser(_reqBuffer[socket].reqString, _reqBuffer[socket].regLength);
		_reqBuffer[socket].reqString = "";
		_reqBuffer[socket].regLength = 0;
		Response response = Response(request, config);
		char *responseStr = const_cast<char *>(response.getResponse().c_str());
		std::cout << CYAN << response.getResponseCode() << RESET << std::endl;
		int ret = send(_fds[socket].fd, responseStr, strlen(responseStr), 0);
		if (ret < 0) {
			std::cout << "send() failed" << std::endl;
			return ;
		}
		_fds[socket].events = POLLIN;
	}
	catch (RequestParser::UnsupportedMethodException &e ) {
		std::cout << e.what() << std::endl;
	}
}

void	Server::runServer(int timeout,  ServerConfig &config) {
	_fds[0].fd = _listenSocket;
	_fds[0].events = POLLIN;
	this->setTimeout(timeout);
	this->_numberFds = 1;
	int currentSize = 0;
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
				if ( _fds[i].fd == _listenSocket  && POLLIN)
					acceptConnection();
				if ( _fds[i].fd != _listenSocket  && POLLIN)
					receiveRequest(i);
				if (POLLOUT)
					handleRequest(i, config);
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

bool Server::findReqEnd(std::string requestBuffer, size_t requestLength) {
	std::string method = requestBuffer;
	method = method.substr(0, requestBuffer.find_first_of(' '));
	if (requestBuffer[requestLength - 1] == '\n' &&
	requestBuffer[requestLength - 2] == '\r' &&
	requestBuffer[requestLength - 3] == '\n' &&
	requestBuffer[requestLength - 4] == '\r') {
		if (method != "POST" || requestBuffer[requestLength - 5] == '0')
			return true;
	}
	return false;
}

void	interruptHandler(int sig_int) {
	(void)sig_int;
	std::cout << BgMAGENTA << "\nAttention! Interruption signal caught. Web server stopped\n";
	exit (EXIT_SUCCESS);
}
