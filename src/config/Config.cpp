#include "Config.hpp"

Config::Config(/* args */)
{
}

Config::~Config()
{
}

void Config::set_config_from_file(int argc, char **argv)
{
	std::string config_file;

	if (argc > 1)
		config_file = argv[1];
	else
		config_file = DEFAULT_CONFIG_PATH;
}

void Config::set_simple_config()
{
	ports.insert(DEFAULT_PORT);
	// std::cout << "created p;ort" << std::endl;

	servers[DEFAULT_PORT].push_back(ServerConfig());


	std::cout << "Created simple server config\n" << servers[DEFAULT_PORT][0] << std::endl;
}


const std::set<int> &Config::getPorts() const {
	return ports;
}

const std::map<int, std::vector<ServerConfig> > &Config::getServers() const {
	return servers;
}