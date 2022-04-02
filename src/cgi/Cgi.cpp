#include "Cgi.hpp"
#include <algorithm>

Cgi::Cgi(ServerConfig &serv, RequestParser &req) {
	if (DEBUG > 1) {
		req.showHeaders();
		_message << BLUE << "first 500 from total " << req.getBody().size() << ":\n" << RESET << req.getBody().substr(0, 500) << std::endl;
		Logger::printDebugMessage(&_message);
	}
	_env["SERVER_NAME"] = "webserv"; //serv.getHost();
	_env["SERVER_SOFTWARE"] = "C.y.b.e.r.s.e.r.v/0.077";
	_env["GATEWAY_INTERFACE"] = "CGI/1.1";

	_env["SERVER_PROTOCOL"] = "HTTP/1.1";
	_env["SERVER_PORT"] = numberToString(serv.getPort());
	_env["REQUEST_METHOD"] = req.getMethod(); // req.getMethod()
	_env["REQUEST_URI"] = req.getPathInfo(); // + req.getQuery(); // req.getRoute() + req.getQuery()
	_env["PATH_INFO"] = req.getPathInfo();
	_env["PATH_TRANSLATED"] = req.getPathTranslated();
	_env["REDIRECT_STATUS"] = "200"; // ??? opyat kakayato hueta
	_env["SCRIPT_NAME"] = serv.getCgi();
	_env["QUERY_STRING"] =  req.getQuery();// req.getQuery();

	_env["AUTH_TYPE"] = ""; //bonus or hz
	_env["REMOTE_IDENT"] = ""; //bonus
	_env["REMOTE_USER"] = ""; //bonus
	_env["REMOTE_ADDR"] = "127.0.0.1";
	_env["CONTENT_TYPE"] = req.getContentType();
	_env["CONTENT_LENGTH"] =  numberToString(req.getBody().size());

	_body = req.getBody();
	_headers = req.getHeaders();
	_message.clear();

}

char ** Cgi::getNewEnviroment() const {
	extern char		**environ;

	std::map<std::string, std::string>::const_iterator it;
	for (it = _env.begin(); it != _env.end(); it++)
		setenv(it->first.c_str(), it->second.c_str(), 1);
	for (it = _headers.begin(); it != _headers.end(); it++)
		setenv(("HTTP_" + it->first).c_str(), it->second.c_str(), 1);
	return environ;
}

std::pair<int, std::string> Cgi::execute() {
	
	std::pair <int, std::string> simple_sgi;
	simple_sgi.first = 500;
	// simple_sgi.second = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 18\n\r\n\rOur sgi is working";
	// return simple_sgi;
	int		pid;
	FILE	*fsInput = tmpfile();
    FILE	*fsOutput = tmpfile();

    int fdInput = fileno(fsInput);
    int fdOutput = fileno(fsOutput);


	if (_body.size()) {
		ssize_t record;
		record = write(fdInput, _body.c_str(), _body.size());
		if (record <= 0)
		{
			_message << "CGI input file record error";
			Logger::printCriticalMessage(&_message);
			return _error500(fdInput, fdOutput, fsInput, fsOutput);
		}
    	lseek(fdInput, 0, SEEK_SET);
	}

	pid = fork();
	if (!pid){ 
		char	**envs;
		char	*args[4];
		std::stringstream str;

		envs = getNewEnviroment();
		bzero(args, sizeof(*args) * 4);
		args[0] = (char *)_env["SCRIPT_NAME"].c_str();
		args[1] = (char *)_env["PATH_TRANSLATED"].c_str();
		if (DEBUG > 1) {
			_message << RED << "RUN SGI!!: " << RESET << args[0]
					<< "\n path_name: " << getenv("PATH_INFO") 
					<< "\n content_lenght: " << getenv("CONTENT_LENGTH") 
					<< "\n real size: " << _body.size() 
					<< "\n content type: " << getenv("CONTENT_TYPE");
			Logger::printDebugMessage(&str);
		}

		if (dup2(fdInput, STDIN_FILENO) < 0 || dup2(fdOutput, STDOUT_FILENO) < 0)
			exit(3);

		execve(args[0], (char *const *)args, envs);
		exit(5);
	}

	// startTime = clock();
    _env.clear();

	

	int closeCode = 0;
	waitpid(pid, &closeCode, 0);
	closeCode = WEXITSTATUS(closeCode);
	// if (DEBUG > 2)
	// 	printf("dogdalis' pid=%d, closecode=%d\n", pid, closeCode);
	if (closeCode)
		return _error500(fdInput, fdOutput, fsInput, fsOutput);
		
	std::string answer;
	std::string tail;
	char buffer[SIZE_BUF_TO_RCV];
	int recived = 1;
	lseek(fdOutput, 0, SEEK_SET);
	while (recived) {
		recived = read(fdOutput, buffer, SIZE_BUF_TO_RCV);
		// if (DEBUG > 2)
		// 	printf("считали %d байт с %d\n", recived, fdOutput);
		if (recived < 0) {
			if (DEBUG > 0)
			{
				_message << "CGI output file reading error";
				Logger::printCriticalMessage(&_message);
				return _error500(fdInput, fdOutput, fsInput, fsOutput);
			}
			close (fdOutput);
			return _error500(fdInput, fdOutput, fsInput, fsOutput);
		}
		else if (recived) {
			tail = std::string(buffer, recived);
			answer += tail;
		}
	}
	close(fdInput);
	close(fdOutput);
	fclose(fsInput);
	fclose(fsOutput);
	simple_sgi.first = 200;
	simple_sgi.second = answer;

	return simple_sgi;
}

std::pair <int, std::string> Cgi::_error500(int fdInput, int fdOutput, FILE *f1, FILE *f2) {
	std::pair <int, std::string> sgi_answer;

	sgi_answer.first = 500;
	sgi_answer.second = "Status: 500 Internal server error";
	// "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 18\n\r\n\rOur sgi is working"
	fclose(f1);
	fclose(f2);
	close(fdInput);
	close(fdOutput);
	return sgi_answer;
}


// int		Cgi::getCgiOut() const {return cgiOut; }
