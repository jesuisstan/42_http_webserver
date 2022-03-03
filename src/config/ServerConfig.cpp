#include "ServerConfig.hpp"

ServerConfig::ServerConfig(std::ifstream &ifs)
{
	std::string cmnd;
	port = 0;
	host = "";

	if (!(ifs >> cmnd) or cmnd != "{")
		baseError("Error while parcing server config :" + cmnd);
	while ((ifs >> cmnd))
	{
		if (cmnd == "listen")
			setPort(ifs);
		else if (cmnd == "host")
			setHost(ifs);
		else if (cmnd == "server_name")
			setServerName(ifs);
		else if (cmnd == "error_page")
			setErrorPage(ifs);
		else if (cmnd == "location")
			setLocation(ifs);
		else if (cmnd == "}")
		{
			if (port and host != "" and locations.size() > 0)
			{
				std::cout << "got server" << host << ":" << port << std::endl;
				return;
			}
		}
		else
			baseError("unrecognized keyWord in config: " + cmnd);
	}
	baseError("Error while parcing server config");
}

// ServerConfig::ServerConfig(
// 		const std::string &host,
// 		const std::string &serverName,
// 		int			port,
// 		std::map<int, std::string> &errorPages,
// 		std::map<std::string, Location> &locations) :
// 	host(host), serverName(serverName), port(port),
// 	errorPages(errorPages), locations(locations) {}



void ServerConfig::setPort(std::istream &ifs)
{
	if (!(ifs >> port))
		baseError("Failed parsing port");
	if (port < 1 or port > 65535)
		baseError("Invalid port number");
	readSemicolon(ifs);
	std::cout << "parced port " << port << std::endl;
}

void ServerConfig::setHost(std::istream &ifs)
{
	if (!(ifs >> host))
		baseError("Failed parsing host");
	host = cutSemicolon(host);
	std::cout << "parced host " << host << std::endl;
}

void ServerConfig::setServerName(std::istream &ifs)
{
	if (!(ifs >> serverName))
		baseError("Failed parsing serverName");
	serverName = cutSemicolon(serverName);
	std::cout << "parced serverName " << serverName << std::endl;
}

void ServerConfig::setErrorPage(std::istream &ifs)
{
	int code;
	std::string path_to_page;

	if (!(ifs >> code) or !(ifs >> path_to_page))
		baseError("Failed parsing setErrorPage");
	if (errorPages.find(code) != errorPages.end())
		baseError("Double errorPage config for code" + numberToString(code));
	path_to_page = cutSemicolon(path_to_page);
	errorPages[code] = path_to_page;
	std::cout << "parced " << code << ":" << path_to_page << std::endl;
}

void ServerConfig::setLocation(std::istream &ifs)
{
	std::string path;

	if (!(ifs >> path))
		baseError("Failed parsing path for Location");
	if (locations.find(path) != locations.end())
		baseError("Double locations config for path" + path);
	locations[path] = Location(ifs);
}


const std::string &ServerConfig::getHost() const { return host; }

const std::string &ServerConfig::getServerName() const { return serverName; }

const int &ServerConfig::getPort() const { return port; }

const std::map<int, std::string> &ServerConfig::getErrorPages() const { return errorPages; }

const std::map<std::string, Location> &ServerConfig::getLocations() const { return locations; }


std::ostream &operator<<(std::ostream &out, const ServerConfig &sc)
{
	out << "ServerConfig\n\thost: " << sc.getHost()
		<< "\n\tserverName: " << sc.getServerName()
		<< "\n\tPort: " << sc.getPort()
		<< "\n\tErrorPages:";
	std::map<int, std::string> pages = sc.getErrorPages();
	std::map<int, std::string>::iterator pi;
	for (pi = pages.begin(); pi != pages.end(); pi++)
		out << "\n\t\t" << pi->first << ' ' << pi->second;

	out << "\n\tLocations:";
	std::map<std::string, Location> locs = sc.getLocations();
	std::map<std::string, Location>::iterator it;
	for (it = locs.begin(); it != locs.end(); it++)
		out << "\n\t" << it->first << '\n'
			<< it->second;

	out << std::endl;

	return out;
}
