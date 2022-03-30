#include "Server.hpp"

Server::Server() {
	memset(&_servAddr, 0, sizeof(_servAddr));
	// memset(_fds, 0, sizeof(_fds));
}

Server::~Server() { //todo rework cleaning
	for (size_t i = 0; i < _fds.size(); i++) {
		if (_fds[i].fd >= 0)
		{
			close(_fds[i].fd);
			pthread_mutex_lock(&g_write);
			std::cerr << BgMAGENTA << "Web server [" << this->serverID <<
					"] successfully closed on socket " << _fds[i].fd << " (D)" << RESET << std::endl;
			pthread_mutex_unlock(&g_write);
		}
	}
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

// int		Server::getNumberFds(void) const {
// 	return (this->_numberFds);
// }

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
	t_reqData req;

	req.reqLength = 0;
	req.reqString = "";
	// req.request = NULL;
	req.response = NULL;
	req.isTransfer = false;
	req.foundHeaders = false;
	req.method = "";	
	req.chunkInd = 0;
	_clients[clientFD] = req;
	return ;
}

void	Server::runServer(int timeout) {
	struct pollfd	*fdsBeginPointer;
	pollfd			new_Pollfd = {_listenSocket, POLLIN, 0};

	_fds.push_back(new_Pollfd);
	this->setTimeout(timeout);
	while (true) {
		if (DEBUG > 0) {
			pthread_mutex_lock(&g_write);
			std::cerr << "Waiting on poll() [server " << this->serverID << "]...\n";
			pthread_mutex_unlock(&g_write);
		}
		fdsBeginPointer = &_fds[0];
		int ret = poll(fdsBeginPointer, _fds.size(), _timeout);
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
		for (size_t i = 0; i < _fds.size(); i++) {
			if (_fds[i].revents == 0)
				continue;
			if (_fds[i].revents) {
				if (_fds[i].revents & POLLIN && _fds[i].fd == _listenSocket)
					acceptConnection();
				// if ()
				else if (_fds[i].revents & POLLIN && _fds[i].fd != _listenSocket)
					receiveRequest(_fds[i]);
				else if (_fds[i].revents & POLLOUT && _fds[i].fd != _listenSocket)
					sendResponse(_fds[i]);
				else
					pollError(_fds[i]);
			}
		}
		clearConnections();
		// usleep(10000); // todo включить если тестируете через curl большие файлы
	}
	return ;
}


void	Server::acceptConnection(void) {
	int newFd = accept(_listenSocket, NULL, NULL);
	if (newFd < 0)
		return ;
	int ret = fcntl(newFd, F_SETFL, fcntl(newFd, F_GETFL, 0) | O_NONBLOCK);
	if (ret < 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << "fcntl() failed" << " on server " << this->serverID << std::endl;
		pthread_mutex_unlock(&g_write);
		close(_listenSocket);
		exit(-1);
	}
	pthread_mutex_lock(&g_write);
	std::cerr << "New incoming connection:\t" << newFd << " on server " << this->serverID << std::endl;
	pthread_mutex_unlock(&g_write);

	pollfd	newConnect = {newFd, POLLIN, 0};
	_fds.push_back(newConnect);
	initReqDataStruct(newFd);
	return ;
}

void	Server::clearConnections() {
	int fd;
	std::vector<struct pollfd>::iterator it;

	it = _fds.begin();

	for (size_t i = 0; i < _fds.size(); i++)
		if (_fdToDel.count(_fds[i].fd))
		{
			fd = _fds[i].fd;
			close(fd);
			if (DEBUG > 0) {
				pthread_mutex_lock(&g_write);
				std::cerr << "Connection has been closed:\t" << fd << " on server " << this->serverID << std::endl;
				pthread_mutex_unlock(&g_write);
			}
			if (_clients[fd].response)
				delete _clients[fd].response;
			_fdToDel.erase(fd);
			_clients.erase(fd);
			_fds.erase(it + i);
			--i;
		}
}

void	Server::receiveRequest(pollfd &pfd) {
	if (DEBUG > 1) {
		pthread_mutex_lock(&g_write);
		std::cerr << "Event detected on descriptor:\t" << pfd.fd << " on server " << this->serverID << std::endl;
		pthread_mutex_unlock(&g_write);
	}
	int ret = 0;
	char buffer[BUFFER_SIZE];
	ret = recv(pfd.fd, buffer, BUFFER_SIZE, 0);
	if (ret > 0)
	{
		std::string tail = std::string(buffer, ret);
		_clients[pfd.fd].reqLength += ret;
		_clients[pfd.fd].reqString += tail;
		if (DEBUG > 1) {
			pthread_mutex_lock(&g_write);
			std::cerr << _clients[pfd.fd].reqLength << " bytes received from sd:\t" << pfd.fd << " on server " << this->serverID <<  std::endl;
			pthread_mutex_unlock(&g_write);
		}
		// memset(buffer, 0, BUFFER_SIZE);
		if (findReqEnd(_clients[pfd.fd]))
			pfd.events = POLLOUT;
	}
	if (ret == 0 || ret == -1) {
		
		_fdToDel.insert(pfd.fd);
		if (DEBUG > 1) {
			if (!ret) {
				pthread_mutex_lock(&g_write);
				std::cerr << "Request to close connection:\t" << pfd.fd << " on server " << this->serverID << std::endl;
				pthread_mutex_unlock(&g_write);
			}
			else {
				pthread_mutex_lock(&g_write);
				std::cerr << "recv() failed" << " on server " << this->serverID << std::endl;
				pthread_mutex_unlock(&g_write);
			}
		}
	}
	return ;
}

void	Server::sendResponse(pollfd &pfd) {
	// if (_fds[socket].revents & POLLNVAL or _fds[socket].revents & POLLHUP or _fds[socket].revents & POLLERR) 
	// 	return;
	if (!_clients[pfd.fd].response) {
		try {
			RequestParser request = RequestParser(_clients[pfd.fd].reqString, _clients[pfd.fd].reqLength);
			if (DEBUG > 0) {
				pthread_mutex_lock(&g_write);
				if (request.getBody().length() > 10000)
					request.showHeaders();
				else
					std::cerr << "|" << YELLOW  << request.getRequest() << RESET"|" << std::endl;
				pthread_mutex_unlock(&g_write);
			}
			_clients[pfd.fd].reqString = "";
			_clients[pfd.fd].reqLength = 0;
			_clients[pfd.fd].foundHeaders = 0;
			_clients[pfd.fd].chunkInd = 0;
			_clients[pfd.fd].response =  new Response(request, webConfig);
		}
		catch (RequestParser::UnsupportedMethodException &e) {
			pthread_mutex_lock(&g_write);
			std::cerr << e.what() << std::endl;
			pthread_mutex_unlock(&g_write);
		}
	}
	Response *response = _clients[pfd.fd].response;
	const char *responseStr;
	size_t responseSize;
	size_t chunkInd;
	if (_clients[pfd.fd].response->getChunked()) {
		chunkInd = _clients[pfd.fd].chunkInd;
		
		responseStr = &(response->getChunks()[chunkInd][0]);
		responseSize = response->getChunks()[chunkInd].size();
		if (DEBUG > 0) {
			pthread_mutex_lock(&g_write);
			std::cerr << "send chunk " << chunkInd << " data:\n" << response->getChunks()[chunkInd].substr(0, 130) << std::endl;
			pthread_mutex_unlock(&g_write);
		}
		usleep(50000);
		_clients[pfd.fd].chunkInd = ++chunkInd;
	}
	else {
		responseStr = &response->getResponse()[0];
		responseSize = response->getResponse().size();
	}
	if (DEBUG > 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << CYAN << _clients[pfd.fd].response->getResponseCode() << RESET" with size="  << responseSize << std::endl;
		pthread_mutex_unlock(&g_write);
	}
	int ret = send(pfd.fd, responseStr, responseSize, 0);
	if (ret > 0 and ret < (int)responseSize) {
		std::cerr << RED"ret = " << ret << " but must be " << "responceSize"RESET << std::endl;
		throw("AAAAAA");
	}
	// free (responseStr);
	if (ret < 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << "send() failed " << " on server " << this->serverID << std::endl;
		pthread_mutex_unlock(&g_write);
		_fdToDel.insert(pfd.fd);
		return ;
	}
	if (!response->getChunked() or chunkInd == response->getChunks().size()) {
		delete _clients[pfd.fd].response;
		_clients[pfd.fd].response = NULL;
		pfd.events = POLLIN;
	}
	return ;
}


void Server::pollError(pollfd &pfd)
{
	if (DEBUG > 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << "Error in fd = " << pfd.fd << RED ;

		if (pfd.revents & POLLNVAL)
			std::cerr << " POLLNVAL" << std::endl;
		else if (pfd.revents & POLLHUP)
			std::cerr << " POLLHUP" << std::endl;
		else if (pfd.revents & POLLERR)
			std::cerr << " POLLERR"	<< std::endl;
		std::cerr << RESET << std::endl;
		pthread_mutex_unlock(&g_write);
	}
	_fdToDel.insert(pfd.fd);
}

bool Server::isChunked(std::string headers) {
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