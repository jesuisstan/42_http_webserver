#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include "ServerConfig.hpp"

#define DEFAULT_CONFIG_PATH "default.cfg"

class Config
{
private:
	std::set<int> ports;
	std::map<int, std::vector<ServerConfig> > servers;

public:
	Config(/* args */);
	~Config();
	void set_config_from_file(int argc, char **argv);
	void set_simple_config();
	const std::set<int> &getPorts() const;
	const std::map<int, std::vector<ServerConfig> > &getServers() const;
};

#endif