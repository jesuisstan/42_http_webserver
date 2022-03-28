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
	RequestParser	request_;
	std::string		body_;
	std::map<std::string, std::string>	env_;
	// char *script_argv_[4];
	size_t		startTime;
	bool		emptyBody;
	int			cgiOut;  // fd for cgi result

	char	**getNewEnviroment() const;
	std::pair <int, std::string>	error500_(int fdInput, int fdOutput, FILE *f1, FILE *f2);

	

public:
	Cgi(ServerConfig &serv, Location &loca, RequestParser &req);
	std::pair<int, std::string>	execute();
	int exec();

	int	getCgiOut() const;


};

#endif