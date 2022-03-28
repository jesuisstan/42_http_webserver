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
		
	std::ifstream ifs(filename.c_str());
	if (!ifs.is_open())
		baseError("Cannot open config file: " + filename);

	while ((ifs >> keyWord) and !ifs.eof()) {
		if (keyWord != "server")
			baseError("Corrupted data in config file near " + keyWord);
		ServerConfig serverConfig(ifs);
		portHost = numberToString(serverConfig.getPort()) + ":" + serverConfig.getHost();
		if (!this->portHosts.count(portHost))
		{
			this->portHosts.insert(portHost);
			this->servers.push_back(serverConfig);
		}
	}
	ifs.close();
	if (servers.size() < 1)
		baseError("Not found any server configs in file");
}

Config::~Config()
{
}

const std::vector<ServerConfig> &Config::getServers() const {
	return servers;
}
