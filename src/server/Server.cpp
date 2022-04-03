#include "Server.hpp"

Server::Server() {
	memset(&_servAddr, 0, sizeof(_servAddr));
	_cntLargeCgi = 0;
	// memset(_fds, 0, sizeof(_fds));
}

Server::~Server() { //todo rework cleaning
	for (size_t i = 0; i < _fds.size(); i++) {
		if (_fds[i].fd >= 0)
		{
			close(_fds[i].fd);
			// _message << BgMAGENTA << "Web server [" << this->serverID << "]: connection closed on socket "
						// << _fds[i].fd << " (D)" << RESET;
			// Logger::printCriticalMessage(&_message);
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
		// _message << "socket() failed" << " on server " << this->serverID;
		// Logger::printCriticalMessage(&_message);
		exit(-1);
	}
	int optval = 1;
	int ret = setsockopt(this->_listenSocket, SOL_SOCKET, SO_REUSEADDR, (char *)&optval, sizeof(optval));
	if (ret < 0) {
		// _message << "setsockopt() failed" << " on server " << this->serverID;
		// Logger::printCriticalMessage(&_message);
		close(this->_listenSocket);
		exit(-1);
	}
	ret = fcntl(this->_listenSocket, F_SETFL, O_NONBLOCK);
	if (ret < 0) {
		// _message << "fcntl() failed" << " on server " << this->serverID;
		// Logger::printCriticalMessage(&_message);
		close(this->_listenSocket);
		exit(-1);
	}
	this->_servAddr.sin_family = AF_INET;
	this->_servAddr.sin_addr.s_addr = inet_addr(ipAddr);
	this->_servAddr.sin_port = htons(port);
	ret = bind(this->_listenSocket, (struct sockaddr *)&this->_servAddr, sizeof(this->_servAddr));
	if (ret < 0) {
		// _message << "bind() failed" << " on server " << this->serverID;
		// Logger::printCriticalMessage(&_message);
		close(this->_listenSocket);
		exit(-1);
	}
	ret = listen(this->_listenSocket, BACKLOG);
	if (ret < 0) {
		// _message << "listen() failed" << " on server " << this->serverID;
		// Logger::printCriticalMessage(&_message);
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
	req.responseStr = NULL;
	req.responseSize = 0;
	req.isTransfer = false;
	req.isMultipart = false;
	req.foundHeaders = false;
	req.method = "";	
	req.chunkInd = 0;
	req.cntCgi = 0;
	_clients[clientFD] = req;
	return ;
}

void	Server::runServer(int timeout) {
	struct pollfd	*fdsBeginPointer;
	pollfd			new_Pollfd = {_listenSocket, POLLIN, 0};

	_fds.push_back(new_Pollfd);
	this->setTimeout(timeout);
	while (true) {
		// _message <<"Waiting on poll() [server " << this->serverID << "]...\n";
		// Logger::printDebugMessage(&_message);
		fdsBeginPointer = &_fds[0];
		int ret = poll(fdsBeginPointer, _fds.size(), _timeout);
		if (ret < 0) {
			// _message << "poll() failed" << " on server " << this->serverID;
			// Logger::printCriticalMessage(&_message);
			break;
		}
		if (ret == 0) {
			// _message << "poll() timed out. End program." << " on server " << this->serverID; ;
			// Logger::printCriticalMessage(&_message);
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
	int ret = fcntl(newFd, F_SETFL, O_NONBLOCK);
	if (ret < 0) {
		// _message << "fcntl() failed" << " on server " << this->serverID;
		// Logger::printCriticalMessage(&_message);
		close(_listenSocket);
		exit(-1);
	}
	// _message << "New incoming connection:\t" << newFd << " on server " << this->serverID;
	// Logger::printCriticalMessage(&_message);

	pollfd	newConnect = {newFd, POLLIN, 0};
	_fds.push_back(newConnect);
	initReqDataStruct(newFd);
	return ;
}

void	Server::clearConnections() {
	int fd;
	std::vector<struct pollfd>::iterator it;

	if (!_fdToDel.size())
		return ;
	it = _fds.begin();

	for (size_t i = 0; i < _fds.size(); i++)
		if (_fdToDel.count(_fds[i].fd))
		{
			fd = _fds[i].fd;
			close(fd);
			// _message << "Connection has been closed:\t" << fd << " on server " << this->serverID;
			// Logger::printCriticalMessage(&_message);
			if (_clients[fd].response)
				delete _clients[fd].response;
			_fdToDel.erase(fd);
			_clients.erase(fd);
			_fds.erase(it + i);
			--i;
		}
}


void	Server::receiveRequest(pollfd &pfd) {
	// _message << "Event detected on descriptor:\t" << pfd.fd << " on server " << this->serverID;
	// Logger::printDebugMessage(&_message);
	int ret = 0;
	char buffer[BUFFER_SIZE];
	ret = recv(pfd.fd, buffer, BUFFER_SIZE, 0);
	if (ret > 0)
	{
		std::string tail = std::string(buffer, ret);
		_clients[pfd.fd].reqLength += ret;
		_clients[pfd.fd].reqString += tail;
		// _message << _clients[pfd.fd].reqLength << " bytes received from sd:\t" << pfd.fd << " on server " << this->serverID <<  std::endl;
		// Logger::printDebugMessage(&_message);
		// memset(buffer, 0, BUFFER_SIZE);
		if (findReqEnd(_clients[pfd.fd]))
			pfd.events = POLLOUT;
//        std::cout << BgCYAN << "awsdadeafsrfa "<< _clients[pfd.fd].isMultipart << "|" << ((_clients[pfd.fd].reqString.find(_clients[pfd.fd].finalBound) == std::string::npos)) << RESET << std::endl;

	}
	if (ret == 0 || ret == -1) {
		
		_fdToDel.insert(pfd.fd);
		if (!ret) {
			// _message << "Request to close connection:\t" << pfd.fd << " on server " << this->serverID;
			// Logger::printDebugMessage(&_message);;
		}
		else {
			// _message << "recv() failed" << " on server " << this->serverID;
			// Logger::printDebugMessage(&_message);
		}
	}
	return ;
}

void	Server::sendResponse(pollfd &pfd) {
	// if (_fds[socket].revents & POLLNVAL or _fds[socket].revents & POLLHUP or _fds[socket].revents & POLLERR) 
	// 	return;
	if (!_clients[pfd.fd].response) {
		try {
			// sleep(5);
			RequestParser request = RequestParser(_clients[pfd.fd].reqString, _clients[pfd.fd].reqLength);
			if (request.getBody().length() > 10000)
				request.showHeaders();
			else {
				// _message << "|" << YELLOW  << request.getRequest() << RESET"|";
				// Logger::printInfoMessage(&_message);
			}
			_clients[pfd.fd].reqString = "";
			_clients[pfd.fd].reqLength = 0;
			_clients[pfd.fd].foundHeaders = 0;
			_clients[pfd.fd].chunkInd = 0;
			_clients[pfd.fd].response =  new Response(request, webConfig);
		}
		catch (RequestParser::UnsupportedMethodException &e) {
			// _message << e.what();
			// Logger::printCriticalMessage(&_message);
			return ;
		}
	}
	Response *response = _clients[pfd.fd].response;
	const char *responseStr;
	size_t responseSize;
	size_t chunkInd;
	if (_clients[pfd.fd].responseSize) {
		// не получилось отправить в прошлый раз всю строку
		responseStr = _clients[pfd.fd].responseStr;
		responseSize = _clients[pfd.fd].responseSize;
	}
	else if (_clients[pfd.fd].response->getChunked()) {
		// отправляем следующий чанк
		chunkInd = _clients[pfd.fd].chunkInd;
		
		responseStr = &(response->getChunks()[chunkInd][0]);
		responseSize = response->getChunks()[chunkInd].size();
		// _message << "send chunk " << chunkInd << " data:\n" << response->getChunks()[chunkInd].substr(0, 130);
		// Logger::printInfoMessage(&_message);
		// usleep(50000);
		_clients[pfd.fd].chunkInd = ++chunkInd;
	}
	else {
		// отправляем весь ответ целиком
		responseStr = &response->getResponse()[0];
		responseSize = response->getResponse().size();
	}
	// _message << CYAN << _clients[pfd.fd].response->getResponseCode() << RESET" with size="  << responseSize;
	// Logger::printInfoMessage(&_message);
	int ret = send(pfd.fd, responseStr, responseSize, 0x80000);
			usleep(10000);

		// throw("AAAAAA");
	_clients[pfd.fd].responseStr = (char *)responseStr + ret;
	_clients[pfd.fd].responseSize = responseSize - ret;
	if (ret > 0 and ret < (int)responseSize) {
		// _message << RED << "ret = " << ret << " but must be " << "responceSize" << RESET;
		// Logger::printDebugMessage(&_message);
	}
	// free (responseStr);
	if (ret <= 0) {
		// _message << "send() failed " << " on server " << this->serverID;
		// Logger::printCriticalMessage(&_message);
		_fdToDel.insert(pfd.fd);
		return ;
	}
	if (!_clients[pfd.fd].responseSize and (!response->getChunked() or chunkInd == response->getChunks().size())) {
		// if (response->getChunks().size())
		// 	std::cerr << GREEN"Sended total large CGI: " << _cntLargeCgi << ", fd: " << pfd.fd << RESET << std::endl;
		if (chunkInd == response->getChunks().size()) {
			std::cerr << "Sended total large CGI: " << ++_cntLargeCgi << ", fd: " << pfd.fd << " ,total send from this fd: " << ++_clients[pfd.fd].cntCgi << std::endl;
			if (_cntLargeCgi == 102)
				std::cerr << RED"end tests\n";
			// if (++_clients[pfd.fd].cntCgi == 5)
			// 	_fdToDel.insert(pfd.fd);
		}
		if (_clients[pfd.fd].response) {
			delete _clients[pfd.fd].response;
			_clients[pfd.fd].response = NULL;
		}
		pfd.events = POLLIN;
	}
	return ;
}


void Server::pollError(pollfd &pfd)
{
	// _message << "Error in fd = " << pfd.fd << RED ;

	// if (pfd.revents & POLLNVAL)
		// _message << " POLLNVAL";
	// else if (pfd.revents & POLLHUP)
		// _message << " POLLHUP";
	// else if (pfd.revents & POLLERR)
		// _message << " POLLERR"	<< std::endl;
	// _message << RESET;
	// Logger::printInfoMessage(&_message);
	// close(pfd.fd);
	_fdToDel.insert(pfd.fd);
}

void Server::isChunked(std::string headers, s_reqData *req) {
	size_t startPos = headers.find("Transfer-Encoding:");
	if (startPos != std::string::npos) {
		size_t endLine = headers.find("\n", startPos);
		std::string transferEncodingLine = headers.substr(startPos, endLine - startPos);
		req->isTransfer =  (transferEncodingLine.find("chunked") != std::string::npos);
	}
	startPos = headers.find("Content-Type:");
	if (startPos != std::string::npos) {
		size_t endLine = headers.find("\n", startPos);
		std::string typeLine = headers.substr(startPos, endLine - startPos);
		if (typeLine.find("multipart/form-data;") != std::string::npos) {
			req->isMultipart = true;
			size_t boundaryStart = typeLine.find("boundary=") + 9;
			size_t boundaryEnd = typeLine.length();
			req->bound = typeLine.substr(boundaryStart, boundaryEnd - boundaryStart - 1);
			req->finalBound = req->bound + "--";
		}
		// std::cout << BgBLUE << req->bound << RESET << std::endl;
	}
}

bool Server::endByTimeout(t_reqData &req) {
	// todo add for too long request 
	(void)req;
	return false;
}

bool Server::findReqEnd(t_reqData &req) {
	size_t	pos;
	if (!req.foundHeaders) {
	    size_t headersEnd = req.reqString.find(ENDH);
		if (headersEnd == std::string::npos) // todo заголовок еще не пришел до конца
			return false;
		req.foundHeaders = true;
		req.method = req.reqString.substr(0, req.reqString.find_first_of(' '));
		isChunked(req.reqString.substr(0, headersEnd), &req);
	}
	if (!req.isTransfer && !req.isMultipart)
		return true;
	pos = std::max(0, (int)req.reqString.size() - 100);
	if (req.isTransfer and req.reqString.find("0\r\n\r\n", pos) != std::string::npos)
	{
		std::cerr << YELLOW"new CGI request \n"RESET << req.reqString.substr(0, 500) << std::endl;
		return true;
	}
	if (req.isTransfer and req.method != "POST" and req.method != "PUT" and req.reqString.find(ENDH, pos) != std::string::npos)
		return true;
	if (req.isMultipart && req.reqString.find(req.finalBound, pos) != std::string::npos)
		return true;
	return false;
}