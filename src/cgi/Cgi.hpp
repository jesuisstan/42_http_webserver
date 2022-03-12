#ifndef CGI_HPP
#define CGI_HPP

#include "webserv.hpp"
#include "ServerConfig.hpp"
#include "RequestParser.hpp"
#include "Response.hpp" // ?

class Cgi
{
private:
	std::map<std::string, std::string>	env_;
	char *script_argv_[4];

	char **getNewEnviroment() const;

	

public:
	Cgi(ServerConfig &serv, Location &loca);
	std::pair<int, std::string>	execute();



};

#endif