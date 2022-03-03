#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include "webserv.hpp"
#include "Location.hpp"

#define DEFAULT_ERROR_PAGE "error.html"
#define DEFAULT_PORT 8080

class ServerConfig {
private:
	std::string						_cmnd;
	std::string						host;
	std::string						serverName;
	int								port;
	std::map<int, std::string>		errorPages;
	std::map<std::string, Location>	locations;

public:
	ServerConfig();
	ServerConfig(std::ifstream &ifs);
	// ServerConfig(
	// 	const std::string				&host,
	// 	const std::string				&serverName,
	// 	int								port,
	// 	std::map<int, std::string>		&errorPages,
	// 	std::map<std::string, Location>	&locations
	// );

	const std::string						&getHost() const;
	const std::string						&getServerName() const;
	// const std::string &getPortHost() const;
	const int								&getPort() const;
	const std::map<int, std::string>		&getErrorPages() const;
	const std::map<std::string, Location>	&getLocations() const;

	void									setPort(std::istream &ifs);
	void									setHost(std::istream &ifs);
	void									setServerName(std::istream &ifs);
	void									setErrorPage(std::istream &ifs);
	void									setLocation(std::istream &ifs);

	// void									readSemicolon(std::istream &ifs);
	// std::string								&cutSemicolon(std::string &line);
};

std::ostream& operator<< (std::ostream &out, const ServerConfig &sc);





#endif