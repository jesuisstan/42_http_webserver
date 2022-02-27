#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include "Location.hpp"

#define DEFAULT_ERROR_PAGE "error.html"
#define DEFAULT_PORT 8080

class ServerConfig {
private:
	std::string	host;
	std::string	serverName;
	int			port;
	std::map<int, std::string>	errorPages;
	std::map<std::string, Location>	locations;
public:
	ServerConfig();
	ServerConfig(
		std::string &host,
		std::string &serverName,
		int			port,
		std::map<int, std::string> &errorPages,
		std::map<std::string, Location> &locations
	);

	const std::string &getHost() const;
	const std::string &getServerName() const;
	const int &getPort() const;
	const std::map<int, std::string> &getErrorPages() const;
	const std::map<std::string, Location> &getLocations() const;
};

std::ostream& operator<< (std::ostream &out, const ServerConfig &sc);





#endif