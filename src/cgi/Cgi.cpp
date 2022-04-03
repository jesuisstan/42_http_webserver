#include "Cgi.hpp"

Cgi::Cgi(ServerConfig &serv, RequestParser &req): request_(req)
{
	std::stringstream str;
	req.showHeaders();
	str << BLUE << "first 500 from total " << req.getBody().size() << ":\n" << RESET << req.getBody().substr(0, 500) << std::endl;
	Logger::printDebugMessage(&str);
	env_["SERVER_NAME"] = "webserv"; //serv.getHost();
	env_["SERVER_SOFTWARE"] = "C.y.b.e.r.s.e.r.v/0.077";
	env_["GATEWAY_INTERFACE"] = "CGI/1.1";

	env_["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_["SERVER_PORT"] = numberToString(serv.getPort());
	env_["REQUEST_METHOD"] = req.getMethod();
	env_["REQUEST_URI"] = req.getPathInfo();
	env_["PATH_INFO"] = req.getPathInfo();
	env_["PATH_TRANSLATED"] = req.getPathTranslated();
	env_["REDIRECT_STATUS"] = "200";
	env_["SCRIPT_NAME"] = serv.getCgi();
	env_["QUERY_STRING"] =  req.getQuery();

	env_["AUTH_TYPE"] = "";
	env_["REMOTE_IDENT"] = "";
	env_["REMOTE_USER"] = "";
	env_["REMOTE_ADDR"] = "127.0.0.1";
	env_["CONTENT_TYPE"] = req.getContentType();
	env_["CONTENT_LENGTH"] =  numberToString(req.getBody().size());

	body_ = req.getBody();
}

char ** Cgi::getNewEnviroment() const {
	extern char		**environ;
	std::string		line;

	std::map<std::string, std::string>::const_iterator it;
	for (it = env_.begin(); it != env_.end(); it++)
		setenv(it->first.c_str(), it->second.c_str(), 1);
	for (it = request_.getHeaders().begin(); it != request_.getHeaders().end(); it++)
		setenv(("HTTP_" + it->first).c_str(), it->second.c_str(), 1);
	return environ;
}

std::pair<int, std::string> Cgi::execute() {
	
	std::pair <int, std::string> simple_sgi;
	int			pid;
	FILE		*fsInput = tmpfile();
    FILE		*fsOutput = tmpfile();

	simple_sgi.first = 500;
    int fdInput  = fileno(fsInput);
    int fdOutput = fileno(fsOutput);

	if (body_.size()) {
		ssize_t record;
		record = write(fdInput, body_.c_str(), body_.size());
		if (record <= 0)
			return error500_(fdInput, fdOutput, fsInput, fsOutput);
    	lseek(fdInput, 0, SEEK_SET);
	}

	pid = fork();
	if (!pid){ 
		char				**envs;
		char				*args[4];
		std::stringstream	str;

		envs = getNewEnviroment();
		bzero(args, sizeof(*args) * 4);
		args[0] = (char *)env_["SCRIPT_NAME"].c_str();
		args[1] = (char *)env_["PATH_TRANSLATED"].c_str();
		str << RED << "RUN SGI!!: " << RESET << args[0]
					<< "\n path_name: " << getenv("PATH_INFO") 
					<< "\n content_lenght: " << getenv("CONTENT_LENGTH") 
					<< "\n real size: " << body_.size() 
					<< "\n content type: " << getenv("CONTENT_TYPE");
		Logger::printDebugMessage(&str);

		if (dup2(fdInput, STDIN_FILENO) < 0 || dup2(fdOutput, STDOUT_FILENO) < 0)
			exit(3);
		execve(args[0], (char *const *)args, envs);
		exit(5);
	}

    env_.clear();
	int closeCode = 0;
	waitpid(pid, &closeCode, 0);
	closeCode = WEXITSTATUS(closeCode);
	if (closeCode)
		return error500_(fdInput, fdOutput, fsInput, fsOutput);
		
	std::string answer;
	std::string tail;
	char buffer[SIZE_BUF_TO_RCV];
	int recived = 1;
	lseek(fdOutput, 0, SEEK_SET);
	while (recived) {
		recived = read(fdOutput, buffer, SIZE_BUF_TO_RCV);
		if (recived < 0) {
			return error500_(fdInput, fdOutput, fsInput, fsOutput);
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

std::pair <int, std::string> Cgi::error500_(int fdInput, int fdOutput, FILE *f1, FILE *f2) {
	std::pair <int, std::string> sgi_answer;

	sgi_answer.first = 500;
	sgi_answer.second = "Status: 500 Internal server error";
	fclose(f1);
	fclose(f2);
	close(fdInput);
	close(fdOutput);
	return sgi_answer;
}
