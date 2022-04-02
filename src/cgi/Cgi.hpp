#ifndef CGI_HPP
#define CGI_HPP


#include "../../inc/webserv.hpp"
#include "../config/ServerConfig.hpp"
#include "../request/RequestParser.hpp"
#include <sys/wait.h>

#define SIZE_BUF_TO_SEND	1234
#define SIZE_BUF_TO_RCV		456789
class Cgi
{
private:
	// RequestParser					request_;
	std::string							_body;
	std::map<std::string, std::string>	_headers;
	std::map<std::string, std::string>	_env;
	size_t								_startTime;
	// bool								emptyBody;
	// int								cgiOut;  // fd for cgi result
	std::stringstream					_message;

	char								**getNewEnviroment() const;
	std::pair <int, std::string>		_error500(int fdInput, int fdOutput, FILE *f1, FILE *f2);

	

public:
	Cgi();
	Cgi(ServerConfig &serv, RequestParser &req);
	std::pair<int, std::string>	execute();
	// int exec();

	// int	getCgiOut() const;


};

#endif