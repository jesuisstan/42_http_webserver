#include "Server.hpp"

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
	if (_listenSocket < 0)
	{
		perror("socket() failed");
		exit(-1);
	}
	int optval = 1;
	int ret = setsockopt(this->_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
	if (ret < 0)
	{
		perror("setsockopt() failed");
		close(this->_listenSocket);
		exit(-1);
	}
	ret = fcntl(this->_listenSocket, F_SETFL, fcntl(_listenSocket, F_GETFL, 0) | O_NONBLOCK);
	if (ret < 0)
	{
		perror("fcntl() failed");
		close(this->_listenSocket);
		exit(-1);
	}
	this->_servAddr.sin_family = AF_INET;
	this->_servAddr.sin_addr.s_addr = inet_addr(ipAddr);
	this->_servAddr.sin_port = htons(port);
	ret = bind(this->_listenSocket, (struct sockaddr *)&this->_servAddr, sizeof(this->_servAddr));
	if (ret < 0)
	{
		perror("bind() failed");
		close(this->_listenSocket);
		exit(-1);
	}
	ret = listen(this->_listenSocket, BACKLOG);
	if (ret < 0)
	{
		perror("listen() failed");
		close(this->_listenSocket);
		exit(-1);
	}
}

void	Server::runServer(int timeout) {
	_fds[0].fd = _listenSocket;
	_fds[0].events = POLLIN;
	this->setTimeout(timeout);

	char buffer[BUFFER_SIZE];
	this->_numberFds = 1;
	int currentSize = 0;
	while (g_serverOnAir != false)
	{
		signal(SIGINT, interruptHandler);
		std::cout << "Waiting on poll()...\n";
		int ret = poll(_fds, _numberFds, _timeout);
		if (ret < 0)
		{
			perror("poll() failed");
			break;
		}
		if (ret == 0)
		{
			std::cout << "poll() timed out. End program.\n";
			break;
		}
		currentSize = _numberFds;
		bool compressArray = false;
		for (int i = 0; i < currentSize; i++)
		{
			if (_fds[i].revents == 0)
				continue;
			if (_fds[i].revents && POLLIN)
			{
				if( _fds[i].fd == _listenSocket)
				{
					int new_sd = -1;
					do
					{
						new_sd = accept(_listenSocket, NULL, NULL);
						if (new_sd < 0)
							break;
						ret = fcntl(new_sd, F_SETFL, fcntl(new_sd, F_GETFL, 0) | O_NONBLOCK);
						if (ret < 0)
						{
							perror("fcntl() failed");
							close(_listenSocket);
							exit(-1);
						}
						std::cout << "New incoming connection:\t" << new_sd << std::endl;
						_fds[_numberFds].fd = new_sd;
						_fds[_numberFds].events = POLLIN;
						_numberFds++;
					} while (new_sd != -1);
				}
				else
				{
					std::cout << "Event detected on descriptor:\t" << _fds[i].fd << std::endl;
					int closeConnection = false;
					while (true)
					{
						ret = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
						if (ret < 0)
							break;
						if (ret == 0)
						{
							closeConnection = true;
							break;
						}
						std::cout << ret << " bytes received from sd:\t" << _fds[i].fd <<  std::endl;

						//std::string headers = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 47\n\n";
						//std::string body = "SURPRISE MOTHERF@CKER!\n\nCyberpunk ain't dead!!!\n";
						//std::string resp = headers + body;
						//ret = send(_fds[i].fd, resp.c_str(), resp.length(), 0);
						memset(buffer, 0, BUFFER_SIZE);
						char *response = createResponse(buffer, "index.html");
						ret = send(_fds[i].fd, response, ft_strlen(response), 0);
						if (ret < 0)
						{
							perror("send() failed");
							closeConnection = true;
							break;
						}
					}
					if (closeConnection)
					{
						close(_fds[i].fd);
						std::cout << "Connection has been closed:\t" << _fds[i].fd << std::endl;
						_fds[i].fd = -1;
						compressArray = true;
					}
				}
			}
		}
		if (compressArray)
		{
			compressArray = false;
			for (int i = 0; i < _numberFds; i++)
			{
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
}

void	Server::closeConnections(void) {
	for (int i = 0; i < _numberFds; i++)
	{
		if (_fds[i].fd >= 0)
		{
			close(_fds[i].fd);
			std::cout << "Connection successfully closed:\t" << _fds[i].fd << std::endl;
		}
	}
}

void	interruptHandler(int sig_int) {
	(void)sig_int;
	std::cout << "\nAttention! Interruption signal caught\n";
	g_serverOnAir = false;
}

static void	fillLength(char *header, long size)
{
	int		i = 0;

	while (header[i])
		i++;
	char *len = ft_itoa(size);
	size_t lenSize = ft_strlen(len);
	for (size_t j = 0; j <= lenSize; j++ )
		header[i++] = len[j];
}

char *createResponse(char *buffer, const char *file) {
	struct stat	buf = {};
	const char *str = "HTTP/1.1 200 OK\\nContent-Type: text/html; charset UTF-8\\nContent-Length: ";
	char tmp[BUFFER_SIZE];
	for ( int i = 0 ; i < 75 ; i++)
		buffer[i] = str[i];

	std::cout << file << std::endl;
	int fd = open(file, O_RDONLY);
	fstat(fd, &buf);
	long indexSize = buf.st_size;
	fillLength(buffer, indexSize);
	read(fd, tmp, BUFFER_SIZE);

	close(fd);
	int j = 0;
	int i = 0;
	while(buffer[i])
		i++;
	buffer[i++] = '\n';
	buffer[i] = '\n';
	while (tmp[j])
		buffer[i++] = tmp[j++];
	return buffer;
}
