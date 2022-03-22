#include "webserv.hpp"
#include "Server.hpp"
#include "RequestParser.hpp"
#include "Response.hpp"

bool	g_serverOnAir;

Server::Server() {
	g_serverOnAir = true;
	memset(&_servAddr, 0, sizeof(_servAddr));
	memset(_fds, 0, sizeof(_fds));
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
	int newSD = -1;
	do {
		newSD = accept(_listenSocket, NULL, NULL);
		if (newSD < 0)
			break;
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
	} while (newSD != -1);
}

void	Server::handleConnection(int i, ServerConfig &config, std::string *requestBuffer, size_t  *requestLength) {
	std::cout << "Event detected on descriptor:\t" << _fds[i].fd << std::endl;
	bool closeConnection = false;
	bool compressArray = false;

	while (!closeConnection) {
		char buffer[BUFFER_SIZE] = {0};
		int ret = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
		if (ret <= 0) {
			if (ret == 0)
				closeConnection = true;
			break;
		}
		*requestBuffer += static_cast<std::string>(buffer).substr(0, ret);
		*requestLength += ret;
		std::cout << requestLength << " bytes received from sd:\t" << _fds[i].fd <<  std::endl;
		memset(buffer, 0, BUFFER_SIZE);
		if (findReqEnd(*requestBuffer, *requestLength))
		{
			try {
				RequestParser request = RequestParser(*requestBuffer, *requestLength);
                if (request.getBody().length() > 10000)
                    request.showHeaders();
                else
                    std::cout << "|"YELLOW  << request.getRequest() << RESET"|" << std::endl;
                *requestBuffer = "";
				*requestLength = 0;
				Response response = Response(request, config);
				char *responseStr = const_cast<char *>(response.getResponse().c_str());
				std::cout << CYAN << response.getResponse() << RESET << std::endl;
				ret = send(_fds[i].fd, responseStr, strlen(responseStr), 0);
				if (ret < 0) {
					std::cout << "send() failed" << std::endl;
					closeConnection = true;
					break;
				}
			}
			catch (RequestParser::UnsupportedMethodException &e ) {
				std::cout << e.what() << std::endl;
			}
		}
	}
	if (closeConnection) {
		close(_fds[i].fd);
		std::cout << "Connection has been closed:\t" << _fds[i].fd << std::endl;
		_fds[i].fd = -1;
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

void	Server::runServer(int timeout,  ServerConfig &config) {
//	signal(SIGINT, interruptHandler);
	_fds[0].fd = _listenSocket;
	_fds[0].events = POLLIN;
	this->setTimeout(timeout);
	this->_numberFds = 1;
	int currentSize = 0;
		std::string requestBuffer = "";
	size_t requestLength = 0;
	while (g_serverOnAir != false) {
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
			if (_fds[i].revents && POLLIN) {
				if( _fds[i].fd == _listenSocket)
					acceptConnection();
				else {
					handleConnection(i, config, &requestBuffer, &requestLength);
				}
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

bool Server::findReqEnd(std::string request_buffer, size_t request_len) {
    std::string method = request_buffer;
    method = method.substr(0, request_buffer.find_first_of(' '));
//	std::cout << "|" << request_buffer[request_len - 5] << "|" << std::endl;
    if (request_buffer[request_len - 1] == '\n' &&
        request_buffer[request_len - 2] == '\r' &&
        request_buffer[request_len - 3] == '\n' &&
        request_buffer[request_len - 4] == '\r') {
        if ((method != "POST" && method != "PUT") || request_buffer[request_len - 5] == '0')
            return true;
    }
    return false;
}

void	interruptHandler(int sig_int) {
	(void)sig_int;
	std::cout << "\nAttention! Interruption signal caught\n";
	g_serverOnAir = false;
}
