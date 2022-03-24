#include "Cgi.hpp"

Cgi::Cgi(ServerConfig &serv, Location &loca, RequestParser &req): request_(req)
{
	env_["SERVER_NAME"] = serv.getHost();
	env_["SERVER_SOFTWARE"] = "C.y.b.e.r.s.e.r.v/0.077";
	env_["GATEWAY_INTERFACE"] = "CGI/1.1";

	env_["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_["SERVER_PORT"] = numberToString(serv.getPort());
	env_["REQUEST_METHOD"] = req.getMethod(); // req.getMethod()
	env_["REQUEST_URI"] = req.getRoute() + req.getQuery(); // req.getRoute() + req.getQuery()
	env_["PATH_INFO"] = req.getPathInfo(); // req.getPathInfo()
	env_["REDIRECT_STATUS"] = ""; // ??? opyat kakayato hueta
	env_["SCRIPT_NAME"] = serv.getCgi();
	env_["QUERY_STRING"] =  req.getQuery();// req.getQuery();

	env_["AUTH_TYPE"] = ""; //bonus or hz
	env_["REMOTE_IDENT"] = ""; //bonus
	env_["REMOTE_USER"] = ""; //bonus
	env_["CONTENT_TYPE"] = req.getContentType(); //req.getContentType()
	env_["CONTENT_LENGTH"] =  req.getContentLength(); //req.getContentLength()
	
	emptyBody = req.getBody().empty();
}


char ** Cgi::getNewEnviroment() const {
	char		**env;
	std::string		line;

	env = new char*[env_.size() + 1];
	std::map<std::string, std::string>::const_iterator it;
	size_t i = 0;
	for (it = env_.begin(); it != env_.end(); it++)
	{
		line = it->first + "=" + it->second;
		env[i] = new char[line.size() + 1];
		std::strcpy (env[i++], line.c_str());
	}
	env[i] = NULL;
	return env;
}

std::pair<int, std::string> Cgi::execute() {
	
	FILE	*in = tmpfile();
	FILE	*out = tmpfile();
	std::pair <int, std::string> simple_sgi;
	simple_sgi.first = 200;
	simple_sgi.second = "Our sgi is working";
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
