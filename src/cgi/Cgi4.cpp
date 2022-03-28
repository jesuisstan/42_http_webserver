#include "Cgi.hpp"
#include <algorithm>

Cgi::Cgi(ServerConfig &serv, Location &loca, RequestParser &req): request_(req)
{
	req.showHeaders();
	env_["SERVER_NAME"] = "webserv"; //serv.getHost();
	env_["SERVER_SOFTWARE"] = "C.y.b.e.r.s.e.r.v/0.077";
	env_["GATEWAY_INTERFACE"] = "CGI/1.1";

	env_["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_["SERVER_PORT"] = numberToString(serv.getPort());
	env_["REQUEST_METHOD"] = req.getMethod(); // req.getMethod()
	env_["REQUEST_URI"] = req.getPathInfo(); // + req.getQuery(); // req.getRoute() + req.getQuery()
	env_["PATH_INFO"] = req.getPathInfo();
	env_["PATH_TRANSLATED"] = req.getPathTranslated();
	env_["REDIRECT_STATUS"] = "200"; // ??? opyat kakayato hueta
	env_["SCRIPT_NAME"] = serv.getCgi();
	env_["QUERY_STRING"] =  req.getQuery();// req.getQuery();

	env_["AUTH_TYPE"] = ""; //bonus or hz
	env_["REMOTE_IDENT"] = ""; //bonus
	env_["REMOTE_USER"] = ""; //bonus
	env_["REMOTE_ADDR"] = "127.0.0.1";
	env_["CONTENT_TYPE"] = req.getContentType();
	env_["CONTENT_LENGTH"] =  numberToString(req.getBody().size());

	emptyBody = req.getBody().empty(); // todo del
	body_ = req.getBody();
}

char ** Cgi::getNewEnviroment() const {
	extern	char **environ;
	std::string		line;

	std::map<std::string, std::string>::const_iterator it;
	size_t i = 0;
	std::cerr << GREEN"CGI_PENVS"RESET << std::endl;
	for (it = env_.begin(); it != env_.end(); it++)
		setenv(it->first.c_str(), it->second.c_str(), 1);
	for (it = request_.getHeaders().begin(); it != request_.getHeaders().end(); it++)
		if (!it->first.compare(0, 5, "HTTP_"))
			setenv(it->first.c_str(), it->second.c_str(), 1);
	return environ;
}

std::pair<int, std::string> Cgi::execute() {
	
	std::pair <int, std::string> simple_sgi;
	simple_sgi.first = 500;
	// simple_sgi.second = "HTTP/1.1 200 OK\nContent-Type: text/html\nContent-Length: 18\n\r\n\rOur sgi is working";
	// return simple_sgi;
	int		res, pid;
	int		input[2];
	int		output[2];

	cgiOut = -1; //todo unset
	
	if (pipe(input) < 0 || pipe(output) < 0)
    	return simple_sgi;
	pid = fork();
	if (!pid){ 
		std::cerr << "RUNS!" << std::endl;
		char	**envs;
		char	*args[4];

		envs = getNewEnviroment();
		bzero(args, sizeof(*args) * 4);
		std::string nameScript = env_["SCRIPT_NAME"];
		nameScript = nameScript.substr(nameScript.rfind('/') + 1);
		// args[0] = (char *)nameScript.c_str();
		args[0] = (char *)env_["SCRIPT_NAME"].c_str();
		args[1] = (char *)env_["PATH_TRANSLATED"].c_str();
		std::cerr << "RUNS!!: " << args[0]
				<< "\n path_name: " << getenv("PATH_INFO") 
				<< "\n content_lenght: " << getenv("CONTENT_LENGTH") 
				<< "\n real size: " << body_.size() 
				<< "\n content type: " << getenv("CONTENT_TYPE") << std::endl;

		close(input[1]);
      	close(output[0]);
		if (dup2(input[0], STDIN_FILENO) < 0 || dup2(output[1], STDOUT_FILENO) < 0) {
			close(output[1]);
      		close(input[0]);
			exit(1);
		}
      	close(output[1]);
      	close(input[0]);

		execve(args[0], (char *const *)args, envs);
		exit(5);
	}

	startTime = clock();
	close(input[0]);
    close(output[1]);
    env_.clear();

	if (!emptyBody) {
		char *buf = &body_[0];
		int sended;
		int byte_to_send;
		int left = body_.size();
		printf("надо записать %d в %d\n", left, input[1]);
		while (left > 0) {
			byte_to_send = std::min(SIZE_BUF_TO_SEND, left);
			sended = write(input[1], buf, byte_to_send);
			printf("записали %d байт в %d\n", sended, input[1]);
			if (sended <= 0 or sended > left)
				return simple_sgi;
			left -= sended;
			buf += sended;
			printf("ostalos %d\n", left);
		}
	}
	close(input[1]);
	int closeCode = 0;
	waitpid(pid, &closeCode, 0);

	closeCode = WEXITSTATUS(closeCode);
	printf("dogdalis' pid=%d, closecode=%d\n", pid, closeCode);
	if (closeCode)
		return simple_sgi;
		
	std::string answer;
	std::string tail;
	char buffer[SIZE_BUF_TO_RCV];
	int recived = 1;
	// size_t left = body_.size();
	while (recived) {
		recived = read(output[0], buffer, SIZE_BUF_TO_RCV);
		printf("считали %d байт с %d\n", recived, output[0]);
		if (recived < 0) {
			close (output[0]);
			return simple_sgi;
		}
		else if (recived) {
			tail = std::string(buffer, recived);
			answer += tail;
		}
	}
	close(output[0]);
	simple_sgi.first = 200;
	simple_sgi.second = answer;
	// printf("got from cgi (code:%d) data|\n%s\n|", simple_sgi.first, simple_sgi.second.c_str());

	return simple_sgi;

}

int Cgi::exec() {
	char	**envs = getNewEnviroment();
	char	*args[4];
	int		res, pid;
	int		input[2];
	int		output[2];

	cgiOut = -1; //todo unset
	bzero(args, sizeof(*args) * 4);
	if (env_["SCRIPT_NAME"] == "python")
		args[0] = "sgi_python.py";
	else if (env_["SCRIPT_NAME"] == "tester")
		args[0] = "cgi_tester";
	else
		args[0] = "php?";
	if (pipe(input) < 0 || pipe(output) < 0)
    	return 0;
	pid = fork();
	if (!pid){ 
		
		send(2, args[0], std::strlen(args[0]), 0); // todo del
		close(input[1]);
      	close(output[0]);
		if (dup2(input[0], STDERR_FILENO) < 0 || dup2(output[1], STDOUT_FILENO) < 0) {
			close(output[1]);
      		close(input[0]);
			exit(-1);
		}
      	close(output[1]);
      	close(input[0]);

		char cgi_bin[] = "cgi_bin"; // вообще нихуя не понятно зачем это делать :/ мб чтобы скрипт работал только в его папке
		if (chdir(cgi_bin) < 0) { // todo
			exit(-1);
		}
		execve(args[0], args, envs);
		perror("cgi cann't run");
		exit(-1);
	}

	startTime = clock();
	close(input[0]);
    close(output[1]);
    env_.clear();
	cgiOut = output[0];
	fcntl(cgiOut, F_SETFL, O_NONBLOCK);
	if (emptyBody) {
    	close(input[1]);
		return 0;
	}
	fcntl(input[1], F_SETFL, O_NONBLOCK);
	return input[1];
}

int		Cgi::getCgiOut() const {return cgiOut; }
