#include "Cgi.hpp"



// default values from request 
// location /YoupiBananae/
// alias ./testers/
// cgi_ext = .cgi
// cgi = "python"
// and uri = "/YoupiBananae/alch.sgi?a=1&b=2"
Cgi::Cgi(ServerConfig &serv, Location &loca, RequestParser &req): request_(req)
{
	env_["SERVER_NAME"] = serv.getHost();
	env_["SERVER_SOFTWARE"] = "C.y.b.e.r.s.e.r.v/0.077";
	env_["GATEWAY_INTERFACE"] = "CGI/1.1";

	env_["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_["SERVER_PORT"] = numberToString(serv.getPort());
	env_["REQUEST_METHOD"] = "GET"; // req.getMethod()
	env_["REQUEST_URI"] = "/YoupiBananae/alch.sgi?a=1&b=2"; // req.getRoute() + req.getQuery()
	env_["PATH_INFO"] = "./testers/alch.sgi"; // req.getPathInfo()
	env_["REDIRECT_STATUS"] = ""; // ??? opyat kakayato hueta
	env_["SCRIPT_NAME"] = serv.getCgi();
	env_["QUERY_STRING"] = "a=1&b=2    request.getQuery()"; // req.getQuery();

	env_["AUTH_TYPE"] = ""; //bonus or hz
	env_["REMOTE_IDENT"] = ""; //bonus
	env_["REMOTE_USER"] = ""; //bonus
	env_["CONTENT_TYPE"] = "request.getContentType()"; //req.getContentType()
	env_["CONTENT_LENGTH"] =  "request.getContentLenght()"; //req.getContentLength()
	
	// Еще сюда нужно добавить все поля с запроса, которые начинаются на http
	// идеально, если ты их в мапу считала
	// пример чужого кода 
	/*
	for (std::map<std::string, std::string, ft::comp>::iterator it = req_headers_.begin(); it != req_headers_.end(); it++) {
    if (!it->second.empty()) {
      std::string header = "HTTP_" + ft::to_upper(it->first);
      std::replace(header.begin(), header.end(), '-', '_');
      cgi_env_[header] = it->second;
    }
	*/
	// ну или можно забить хуй)

    // не хочу ничего решать
    // и хуй тоже не хочу
    // вот сиськи
//    .| : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : : :”-'\,,
//    ..\: : : : : : : : : : :'\: : : : : : : : : : : : : :~,,: : : : : : : : : “~-.,_
//    ...\ : : : : : : : : : : :\: /: : : : : : : : : : : : : : : “,: : : : : : : : : : :"~,_
//    ... .\: : : : : : : : : : :\|: : : : : : : : :_._ : : : : : : \: : : : : : : : : : : : :”- .
//    ... ...\: : : : : : : : : : \: : : : : : : : ( O ) : : : : : : \: : : : : : : : : : : : : : '\._
//    ... ... .\ : : : : : : : : : '\': : : : : : : :"*": : : : : : : :|: : : : : : : : : : : : : : : |0)
//    ... ... ...\ : : : : : : : : : '\: : : : : : : : : : : : : : : :/: : : : : : : : : : : : : : : /""
//    ... ... .....\ : : : : : : : : : \: : : : : : : : : : : : : ,-“: : : : : : : : : : : : : : : :/
//    ... ... ... ...\ : : : : : : : : : \: : : : : : : : : _=" : : : : : ',_.: : : : : : : :,-“
//    ... ... ... ... \,: : : : : : : : : \: :"”'~---~”" : : : : : : : : : : : : = :"”~~

	

	// std::vector<string> vector;
	// const char *programname = "abc";

	// const char **argv = new const char* [vector.size()+2];   // extra room for program name and sentinel
	// argv [0] = programname;         // by convention, argv[0] is program name
	// for (int j = 0;  j < vector.size()+1;  ++j)     // copy args
	// 		argv [j+1] = vector[j] .c_str();

	// argv [vector.size()+1] = NULL;  // end of arguments sentinel is NULL

	// execv (programname, (char **)argv);
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
//	FILE *in = tmpfile();
//	FILE *out = tmpfile();

	// тут будет execve
    std::cout << BgBLUE << "getMethod |" << request_.getMethod()<< RESET << std::endl;
    std::cout << BgBLUE << "getRoute |" << request_.getRoute() + request_.getQuery()<< RESET << std::endl;
    std::cout << BgBLUE << "getQuery |" << request_.getQuery()<< RESET << std::endl;
    std::cout << BgBLUE << "getContentType |" << request_.getContentType()<< RESET << std::endl;
    std::cout << BgBLUE << "getContentLength |" << request_.getContentLength()<< RESET << std::endl;
    std::cout << BgBLUE << "getPathInfo |" << request_.getPathInfo()<< RESET << std::endl;
	
	
	
	

	// FILE	*in = tmpfile();
	// FILE	*out = tmpfile();
	char	**envs = getNewEnviroment();
	int		res, pid;

	pid = fork();
	if (!pid){
		if (env_["SCRIPT_NAME"] == "python") {
			const char *args[] = {
				"python3",
				"-m",
				env_["PATH_INFO"].c_str(),
				0};
			execve(args[0], (char **)args, envs);
		}

		else {
			const char *args[] = {
				"php?",
				env_["PATH_INFO"].c_str(),
				0};
			execve(args[0], (char **)args, envs);
		}
		perror("not running");
	}
		



	std::pair <int, std::string> best_sgi;
	best_sgi.first = 200;
	best_sgi.second = "Our sgi is working";
	return best_sgi;

}