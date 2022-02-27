#include "ServerConfig.hpp"

// ServerConfig::ServerConfig(): 
// 		host("127.0.0.1"),
// 		serverName(""),
// 		port(DEFAULT_PORT),
// 		errorPages(DEFAULT_ERROR_PAGE) {
// 	std::set<std::string> setMethods1;
// 	std::set<std::string> setMethods2;
// 	setMethods1.insert("GET");
// 	setMethods2.insert("GET");
// 	setMethods2.insert("PUT");
// 	setMethods2.insert("DELETE");

// 	locations["Youpibanana/"] = Location(setMethods1, "./YoupiBananae/", "", true);
// 	locations["yeah/"] = Location(setMethods2, "./YoupiBananae/yeah/", "", false);
// }

ServerConfig::ServerConfig(
		std::string &host,
		std::string &serverName,
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
		<< "\n\tErrorPage:";
	std::map<int, std::string> pages = sc.getErrorPages();
	std::map<int, std::string>::iterator pi;
	for (pi=pages.begin(); pi!=pages.end(); pi++)
		out << "\n\t" << pi->first << '\n' << pi->second;

	out	<< "\n\tLocations:";

	std::map<std::string, Location> locs = sc.getLocations();
	std::map<std::string, Location>::iterator it;
	for (it=locs.begin(); it!=locs.end(); it++)
		out << "\n\t" << it->first << '\n' << it->second;

	out << std::endl;
 
    return out;
}
