#include "ServerConfig.hpp"

ServerConfig::ServerConfig(std::ifstream &ifs):
		port(DEFAULT_PORT),
		clientMaxBodySize(DEFAULT_MAX_BODY)
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
		else if (cmnd == "cgi")
			setCgi(ifs);
		else if (cmnd == "cgi_ext")
			setCgiExt(ifs);
		else if (cmnd == "}")
		{
			if (host.empty() or not locations.size())
				baseError("Not configure hostName or location");
			if (!errorPages.size())
				errorPages[404] = DEFAULT_ERROR_PAGE;
			if (cgi.empty() + cgiExt.empty() == 1)
				baseError("fill only one from cgi and cgi_extention");
			if (!cgi.empty() and cgi.compare(0, 8, "cgi-bin/"))
				baseError("unknown sgi, use somthing line 'cgi-bin/tester");
			return ;
		}
		else
			baseError("unrecognized keyWord in config: " + cmnd);
	}
	baseError("Error while parcing server config");
}

void ServerConfig::setPort(std::istream &ifs)
{
	if (!(ifs >> port))
		baseError("Failed parsing port");
	if (port < 1 or port > 65535)
		baseError("Invalid port number");
	readSemicolon(ifs);
}

void ServerConfig::setClientMaxBodySize(std::istream &ifs)
{
	if (!(ifs >> _cmnd))
		baseError("Failed parsing port");
	_cmnd = cutSemicolon(_cmnd);
	if (!isPositiveDigit(_cmnd))
		baseError("Invalid bax body size: " + _cmnd);
	clientMaxBodySize = stringToNumber(_cmnd);
}

void ServerConfig::setHost(std::istream &ifs)
{
	if (!(ifs >> host))
		baseError("Failed parsing host");
	host = cutSemicolon(host);
}

void ServerConfig::setServerName(std::istream &ifs)
{
	if (!(ifs >> serverName))
		baseError("Failed parsing serverName");
	serverName = cutSemicolon(serverName);
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
	while ((line >> _cmnd))
	{
		if (_cmnd.size() and _cmnd[_cmnd.size() - 1] == ';')
		{
			page = cutSemicolon(_cmnd);
			found_semicolon = true;
			break ;
		}
		if (!isPositiveDigit(_cmnd))
			baseError("not only digit characters in errorCode: " + _cmnd);
		code = stringToNumber(_cmnd);
		if (code < 400 or code >= 600)
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
	// std::cerr << "parsed location \n" << locations[path];
}

void	ServerConfig::setCgi(std::istream &ifs) {
	if (!(ifs >> cgi))
		baseError("Failed parsing cgi");
	cgi = cutSemicolon(cgi);
	// std::cerr << "parced cgi " << cgi << std::endl;
}

void	ServerConfig::setCgiExt(std::istream &ifs) {
	if (!(ifs >> cgiExt))
		baseError("Failed parsing cgiExt");
	cgiExt = cutSemicolon(cgiExt);
}


const std::string &ServerConfig::getHost() const { return host; }

const std::string &ServerConfig::getServerName() const { return serverName; }

const int &ServerConfig::getPort() const { return port; }

const int &ServerConfig::getClientMaxBodySize() const { return clientMaxBodySize; }

const std::map<int, std::string> &ServerConfig::getErrorPages() const { return errorPages; }

const std::map<std::string, Location> &ServerConfig::getLocations() const { return locations; }

const std::string &ServerConfig::getCgi() const { return cgi; }

const std::string &ServerConfig::getCgiExt() const { return cgiExt; }


ServerConfig::ServerConfig(const ServerConfig &other) {
    *this = other;
}

ServerConfig &ServerConfig::operator=(const ServerConfig &other) {
    if (this != &other) {
        _cmnd = other._cmnd;
        host = other.host;
        serverName = other.serverName;
        port = other.port;
        clientMaxBodySize = other.clientMaxBodySize;
        errorPages = other.errorPages;
        locations = other.locations;
        cgi = other.cgi;
        cgiExt = other.cgiExt;
    }
    return *this;
}

std::ostream &operator<<(std::ostream &out, const ServerConfig &sc)
{
	out << "ServerConfig\n\thost: " << sc.getHost()
		<< "\n\tserverName: " << sc.getServerName()
		<< "\n\tPort: " << sc.getPort()
		<< "\n\tmax_body_size: " << sc.getClientMaxBodySize()
		<< "\n\tcgi: " << sc.getCgi()
		<< "\n\tcgiExt: " << sc.getCgiExt()
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
