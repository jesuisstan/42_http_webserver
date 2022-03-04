#ifndef CONFIG_HPP
#define CONFIG_HPP

#include "webserv.hpp"
#include "ServerConfig.hpp"

#define DEFAULT_CONFIG_PATH "configs/default.cfg"

class Config
{
private:
	std::set<std::string>	portHosts;
	std::vector<ServerConfig>	servers;

	//to del
	// std::set<int> ports;
	// std::map<int, std::vector<ServerConfig> > servers;


public:
	Config(/* args */);
	Config(char *filename);
	Config(int argc, char **argv);
	~Config();
	// void set_config_from_file(int argc, char **argv);
	// void set_config_with_3_servers();
	const std::vector<ServerConfig> &getServers() const;

	//to del
	// const std::set<int> &getPorts() const;
	// const std::map<int, std::vector<ServerConfig> > &getServers() const;
};

#endif