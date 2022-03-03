#include "Config.hpp"

Config::Config(void)
{
}

Config::Config(int argc, char **argv)
{
	std::string	filename;
	std::string	portHost;
	std::string	keyWord;


	if (argc == 1)
		filename = DEFAULT_CONFIG_PATH;
	else
		filename = argv[1];
		
	std::ifstream ifs(filename);
	if (!ifs.is_open())
		baseError("Cannot open config file: " + filename);

	while ((ifs >> keyWord) and !ifs.eof()) {
		if (keyWord != "server")
			baseError("Corrupted data in config file near " + keyWord);
		ServerConfig serverConfig(ifs);
		portHost = numberToString(serverConfig.getPort()) + serverConfig.getHost();
		if (!this->portHosts.count(portHost))
		{
			this->portHosts.insert(portHost);
			this->servers.push_back(serverConfig);
		}
		// break;
	}
	ifs.close();
	if (servers.size() < 1)
		baseError("Not found any server configs in file");

	// return Config(servers, ports);

	
}

Config::~Config()
{
}

const std::vector<ServerConfig> &Config::getServers() const {
	return servers;
}

// void Config::set_config_from_file(int argc, char **argv)
// {
// 	std::string config_file;

// 	if (argc > 1)
// 		config_file = argv[1];
// 	else
// 		config_file = DEFAULT_CONFIG_PATH;
// }

// void Config::set_config_with_3_servers()
// {
// 	// first server in 8080 port
// 	std::map<int, std::string> errorPages;
// 	errorPages[404] = "404_error.html";
// 	errorPages[500] = "server_error.html";
// 	errorPages[502] = "server_error.html";

// 	std::map<std::string, Location>	locations;
// 	std::set<std::string> setMethods0;
// 	std::set<std::string> setMethods1;
// 	std::set<std::string> setMethods2;
// 	std::vector<std::string> setIndex0;
// 	std::vector<std::string> setIndex1;
// 	std::vector<std::string> setIndex2;
// 	setMethods0.insert("GET");
// 	setIndex0.push_back("index.html");
// 	setMethods1.insert("GET");
// 	setIndex1.push_back("index.html");
// 	setIndex1.push_back("index.htm");
// 	setMethods2.insert("GET");
// 	setMethods2.insert("PUT");
// 	setMethods2.insert("DELETE");
// 	setIndex2.push_back("index.html");
// 	locations["/"] = Location(setMethods0, setIndex0, "./configs", "", true);
// 	locations["/Youpibananae"] = Location(setMethods1, setIndex1, "./YoupiBananae", "", true);
// 	locations["/yeah"] = Location(setMethods2, setIndex2, "./YoupiBananae/yeah", "", false);

// 	ServerConfig server1(
// 		"127.0.0.1",
// 		"",
// 		8080,
// 		errorPages,
// 		locations
// 	);

// 	ports.insert(8080);
// 	servers[8080].push_back(server1);
// 	std::cout << "Created first server in 8080 port\n" << servers[DEFAULT_PORT][0] << std::endl;


// 	// second server in 8080 port
// 	std::map<int, std::string> errorPages3;

// 	std::map<std::string, Location>	locations3;
// 	std::set<std::string> setMethods3;
// 	std::vector<std::string> setIndex3;
// 	setMethods3.insert("GET");
// 	setIndex3.push_back("index.html");
	
// 	locations3["/"] = Location(setMethods3, setIndex3, "./testers", "", true);

// 	ServerConfig server2(
// 		"127.0.0.1",
// 		"cyber",
// 		8080,
// 		errorPages3,
// 		locations3
// 	);

// 	ports.insert(8080);
// 	servers[8080].push_back(server2);
// 	std::cout << "Created second server in 8080 port with server name 'cyber'\n" << servers[DEFAULT_PORT][1] << std::endl;


// 	// thirth server in 8081 port
// 	std::map<int, std::string> errorPages4;

// 	std::map<std::string, Location>	locations4;
// 	std::set<std::string> setMethods4;
// 	std::vector<std::string> setIndex4;
// 	setMethods4.insert("GET");
// 	setIndex4.push_back("index.html");
	
// 	locations4["/"] = Location(setMethods4, setIndex4, "/tmp", "", true);

// 	ServerConfig server3(
// 		"127.0.0.1",
// 		"",
// 		8081,
// 		errorPages4,
// 		locations4
// 	);

// 	ports.insert(8081);
// 	servers[8081].push_back(server3);
// 	std::cout << "Created thirth server in 8081 port\n" << servers[8081][0] << std::endl;

// }




// const std::set<int> &Config::getPorts() const {
// 	return ports;
// }

// const std::map<int, std::vector<ServerConfig> > &Config::getServers() const {
// 	return servers;
// }
