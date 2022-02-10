#include "Server.hpp"

static void fillLength(char *header, long size)
{
	int i = 0;

	while (header[i])
		i++;
	char *len = ft_itoa(size);
	size_t lenSize = ft_strlen(len);
	for (size_t j = 0; j <= lenSize; j++)
		header[i++] = len[j];
}

char *createResponse(char *buffer, const char *file)
{
	struct stat buf = {};
	const char *str = "HTTP/1.1 200 OK\\nContent-Type: text/html; charset UTF-8\\nContent-Length: 1778";
	char tmp[BUFFER_SIZE];
	for (int i = 0; i < 75; i++)
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
	while (buffer[i])
		i++;
	buffer[i++] = '\n';
	buffer[i] = '\n';
	while (tmp[j])
		buffer[i++] = tmp[j++];
	return buffer;
}

Server::Server(){}

//Server::Server(const char *ipAddr, int port)
//{
//	this->_serverOnAir = true;
//	this->_servAddr = {};
//	this->_fds[AMMOUNT_FDS] = {};
//}

Server::Server(const std::string &name)
{
	this->_serverName = name;
	this->_serverOnAir = true;
	//this->_servAddr = {};
	//this->_fds[AMMOUNT_FDS] = {};
}

Server::~Server()
{
	for (int i = 0; i < _numberFds; i++)
	{
		if (_fds[i].fd >= 0)
		{
			std::cout << "Connection " << _fds[i].fd << " successfully closed" << std::endl;
			close(_fds[i].fd);
		}
	}
}

void	Server::setServerOnAir(bool status)
{
	this->_serverOnAir = status;
}

void	Server::setTimeout(int timeout)
{
	this->_timeout = timeout;
}

void	Server::initiate(const char *ipAddr, int port)
{
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

void	Server::runServer(int timeout)
{
	_fds[0].fd = _listenSocket;
	_fds[0].events = POLLIN;
	this->setTimeout(timeout);

	char buffer[BUFFER_SIZE];
	this->_numberFds = 1;
	int current_size = 0;
	while (_serverOnAir)
	{
		//signal(SIGINT, interruptHandler); // todo
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
		current_size = _numberFds;
		bool compress_array = false;
		for (int i = 0; i < current_size; i++)
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
						std::cout << "New incoming connection - " << new_sd << std::endl;
						_fds[_numberFds].fd = new_sd;
						_fds[_numberFds].events = POLLIN;
						_numberFds++;
					} while (new_sd != -1);
				}
				else
				{
					std::cout << "Event on descriptor # " << _fds[i].fd << std::endl;
					int close_conn = false;
					while (true)
					{
						int bytes_read = recv(_fds[i].fd, buffer, sizeof(buffer), 0);
						if (bytes_read < 0)
							break;
						if (bytes_read == 0)
						{
							close_conn = true;
							break;
						}
						std::cout << bytes_read << " bytes received" << std::endl;

						//std::string headers = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 47\n\n";
						//std::string body = "SURPRISE MOTHERF@CKER!\n\nCyberpunk ain't dead!!!\n";
						//std::string resp = headers + body;
						//ret = send(_fds[i].fd, resp.c_str(), resp.length(), 0);
						//std::cout << resp.length() << " - resp len\n";
						memset(buffer, 0, BUFFER_SIZE);
						char *response = createResponse(buffer, "index.html");
						ret = send(_fds[i].fd, response, ft_strlen(response), 0);
						if (ret < 0)
						{
							perror("send() failed");
							close_conn = true;
							break;
						}
					}
					if (close_conn)
					{
						close(_fds[i].fd);
						std::cout << "Connection " << _fds[i].fd << " closed\n";
						_fds[i].fd = -1;
						compress_array = true;
					}
				}
			}
		}
		if (compress_array)
		{
			compress_array = false;
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
				}
			}
		}
	}
}

void	Server::closeConnections(void)
{
	for (int i = 0; i < _numberFds; i++)
	{
		if (_fds[i].fd >= 0)
		{
			std::cout << "Connection " << _fds[i].fd << " successfully closed" << std::endl;
			close(_fds[i].fd);
		}
	}
}

//void	interruptHandler(int sig_int) //todo
//{
//	(void)sig_int;
//	std::cout << "\nAttention! Interruption signal caught\n";
//	Server::setServerOnAir(false);
//}