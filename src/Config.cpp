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
	t_server_config * first_server = new s_server_config;
	std::vector<s_server_config *> vector_servers;

	vector_servers.push_back(first_server);

	// first_server->
	
	this->ports.insert(DEFAULT_PORT);
	this->servers[DEFAULT_PORT] = vector_servers;

	std::cout << "Created simple server config" << this->servers[DEFAULT_PORT][0]->host << ":" << this->servers[DEFAULT_PORT][0]->port << "\n";
}