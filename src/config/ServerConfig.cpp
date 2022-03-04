#include "ServerConfig.hpp"

ServerConfig::ServerConfig(std::ifstream &ifs):
		port(DEFAULT_PORT),
		clientMaxBodySize(1024*1024)
{
	std::string cmnd;

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
		else if (cmnd == "client_max_body_size")
			setClientMaxBodySize(ifs);
		else if (cmnd == "}")
		{
			if (!host.empty() and locations.size() > 0)
			{
				// std::cout << "got server" << host << ":" << port << std::endl;
				if (!errorPages.size())
					errorPages[404] = DEFAULT_ERROR_PAGE;
				return;
			}
			else
				baseError("Not configure hostName or location");
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
	// std::cout << "parced port " << port << std::endl;
}

void ServerConfig::setClientMaxBodySize(std::istream &ifs)
{
	if (!(ifs >> _cmnd))
		baseError("Failed parsing port");
	_cmnd = cutSemicolon(_cmnd);
	if (!isPositiveDigit(_cmnd))
		baseError("Invalid bax body size: " + _cmnd);
	clientMaxBodySize = stringToNumber(_cmnd);
	// std::cout << "parced port " << port << std::endl;
}

void ServerConfig::setHost(std::istream &ifs)
{
	if (!(ifs >> host))
		baseError("Failed parsing host");
	host = cutSemicolon(host);
	// std::cout << "parced host " << host << std::endl;
}

void ServerConfig::setServerName(std::istream &ifs)
{
	if (!(ifs >> serverName))
		baseError("Failed parsing serverName");
	serverName = cutSemicolon(serverName);
	// std::cout << "parced serverName " << serverName << std::endl;
}

void ServerConfig::setErrorPage(std::istream &ifs)
{
	std::stringstream	line;
	std::set<int>		codes;
	std::string			page;
	int					code;
	bool				found_semicolon;

	found_semicolon = false;
	std::getline(ifs, _cmnd);
	if (_cmnd.empty())
		baseError("Error read errorPages");
	line << _cmnd;
	// std::cout << "errorPages line " << line.str() << std::endl;
	while ((line >> _cmnd))
	{
		// std::cout << _cmnd << std::endl;
		if (_cmnd.size() and _cmnd[_cmnd.size() - 1] == ';')
		{
			page = cutSemicolon(_cmnd);
			found_semicolon = true;
			break ;
		}
		if (!isPositiveDigit(_cmnd))
			baseError("not only digit characters in errorCode: " + _cmnd);
		code = stringToNumber(_cmnd);
		if (code < 100 or code >= 600)
			baseError("Incorrect errorCode in " + line.str());
		if (codes.count(code))
			baseError("Dublicated code in errorPages");
		codes.insert(code);
	}
	if (!found_semicolon)
		baseError("Failed parsing errorPages");
	if (!line.eof())
		baseError("Unprocessing parametrs after ';'"  + _cmnd);
	if (!codes.size())
		baseError("Not any code for errorPage");
	for (std::set<int>::iterator it = codes.begin(); it != codes.end(); ++it)
	{
		if (errorPages.count(*it))
			baseError("Doubled errorPages code's in different lines");
		errorPages[*it] = page;
	}
}

void ServerConfig::setLocation(std::istream &ifs)
{
	std::string path;

	if (!(ifs >> path))
		baseError("Failed parsing path for Location");
	if (locations.find(path) != locations.end())
		baseError("Double locations config for path" + path);
	locations[path] = Location(ifs);
	// std::cout << "parsed location \n" << locations[path];
}


const std::string &ServerConfig::getHost() const { return host; }

const std::string &ServerConfig::getServerName() const { return serverName; }

const int &ServerConfig::getPort() const { return port; }

const size_t &ServerConfig::getClientMaxBodySize() const { return clientMaxBodySize; }

const std::map<int, std::string> &ServerConfig::getErrorPages() const { return errorPages; }

const std::map<std::string, Location> &ServerConfig::getLocations() const { return locations; }


std::ostream &operator<<(std::ostream &out, const ServerConfig &sc)
{
	out << "ServerConfig\n\thost: " << sc.getHost()
		<< "\n\tserverName: " << sc.getServerName()
		<< "\n\tPort: " << sc.getPort()
		<< "\n\tmax_body_size: " << sc.getClientMaxBodySize()
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
