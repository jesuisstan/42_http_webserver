#include "Cgi.hpp"



// default values from request 
// location /YoupiBananae/
// alias ./testers/
// cgi_ext = .cgi
// cgi = "python"
// and uri = "/YoupiBananae/alch.sgi?a=1&b=2"
Cgi::Cgi(ServerConfig &serv, Location &loca)
{
	env_["SERVER_NAME"] = serv.getHost();
	env_["SERVER_SOFTWARE"] = "C.y.b.e.r.s.e.r.v/0.077";
	env_["GATEWAY_INTERFACE"] = "CGI/1.1";

	env_["SERVER_PROTOCOL"] = "HTTP/1.1";
	env_["SERVER_PORT"] = numberToString(serv.getPort());
	env_["REQUEST_METHOD"] = "GET"; // todo Alina 
	env_["REQUEST_URI"] = "/YoupiBananae/alch.sgi?a=1&b=2"; // todo Alinaka
	env_["PATH_INFO"] = "./testers/alch.sgi"; //todo Alin
	env_["REDIRECT_STATUS"] = ""; // ??? opyat kakayato hueta
	env_["SCRIPT_NAME"] = loca.getCgi();
	env_["QUERY_STRING"] = "a=1&b=2    request.getQuery()"; // todo ymanfryd

	env_["AUTH_TYPE"] = ""; //bonus or hz
	env_["REMOTE_IDENT"] = ""; //bonus
	env_["REMOTE_USER"] = ""; //bonus
	env_["CONTENT_TYPE"] = "request.getContentType()"; //todo Alinka
	env_["CONTENT_LENGTH"] =  "request.getContentLenght()"; //todo Alinochka
	
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

	if (loca.getCgi() == "python") { // ебанный стыд. напомните сжечь этот код
		script_argv_[0] = "python3";
		script_argv_[1] = "-m";
		script_argv_[2] = "./testers/alch.sgi"; // todo replace to get_path_info
		script_argv_[3] = 0;
	}
}


char **Cgi::getNewEnviroment() const {
	char			**env;
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
	FILE *in = tmpfile();
	FILE *out = tmpfile();

	// тут будет execve

	std::pair <int, std::string> best_sgi;

	best_sgi.first = 200;
	best_sgi.second = "Our sgi is working";

	return best_sgi;

}