#ifndef _serverConfigHPP
# define _serverConfigHPP

#include "webserv.hpp"
#include "Location.hpp"
extern pthread_mutex_t g_write;

class ServerConfig {
private:
	std::string						_cmnd;
	std::string						host;
	std::string						serverName;
	int								port;
	int								clientMaxBodySize;
	std::map<int, std::string>		errorPages;
	std::map<std::string, Location>	locations;
	std::string						cgi;
	std::string						cgiExt;


public:
	ServerConfig() {};
	ServerConfig(const ServerConfig &other);
	ServerConfig &operator=(const ServerConfig &other);
	ServerConfig(std::ifstream &ifs);
	const std::string						&getHost() const;
	const std::string						&getServerName() const;
	const int								&getPort() const;
	const int								&getClientMaxBodySize() const;
	const std::map<int, std::string>		&getErrorPages() const;
	const std::map<std::string, Location>	&getLocations() const;
	const std::string						&getCgi() const;
	const std::string						&getCgiExt() const;
	void									setPort(std::istream &ifs);
	void									setHost(std::istream &ifs);
	void									setServerName(std::istream &ifs);
	void									setClientMaxBodySize(std::istream &ifs);
	void									setErrorPage(std::istream &ifs);
	void									setLocation(std::istream &ifs);
	void									setCgi(std::istream &ifs);
	void									setCgiExt(std::istream &ifs);
};

std::ostream& operator<< (std::ostream &out, const ServerConfig &sc);

#endif
