#include "ServerConfig.hpp"

ServerConfig::ServerConfig(
		const std::string &host,
		const std::string &serverName,
		int			port,
		std::map<int, std::string> &errorPages,
		std::map<std::string, Location> &locations) :
	host(host), serverName(serverName), port(port), 
	errorPages(errorPages), locations(locations) {}


const std::string &ServerConfig::getHost() const{
	return host;
}

const std::string &ServerConfig::getServerName() const {
	return serverName;
}

const int &ServerConfig::getPort() const {
	return port;
}

const std::map<int, std::string> &ServerConfig::getErrorPages() const {
	return errorPages;
}

const std::map<std::string, Location> &ServerConfig::getLocations() const {
	return locations;
}


std::ostream& operator<< (std::ostream &out, const ServerConfig &sc)
{
    out << "ServerConfig\n\thost: " << sc.getHost()
		<< "\n\tserverName: " << sc.getServerName() 
		<< "\n\tPort: " << sc.getPort()
		<< "\n\tErrorPages:";
	std::map<int, std::string> pages = sc.getErrorPages();
	std::map<int, std::string>::iterator pi;
	for (pi=pages.begin(); pi!=pages.end(); pi++)
		out << "\n\t\t" << pi->first << ' ' << pi->second;

	out	<< "\n\tLocations:";
	std::map<std::string, Location> locs = sc.getLocations();
	std::map<std::string, Location>::iterator it;
	for (it=locs.begin(); it!=locs.end(); it++)
		out << "\n\t" << it->first << '\n' << it->second;

	out << std::endl;
 
    return out;
}
