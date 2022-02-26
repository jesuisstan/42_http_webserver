#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <map>
#include <set>
#include <iostream>
#include "ServerConfig.hpp"

#define DEFAULT_CONFIG_PATH "default.cfg"





class Config
{
private:
	std::set<int> ports;
	std::map<int, std::vector<s_server_config *> > servers;

public:
	Config(/* args */);
	~Config();
	void set_config_from_file(int argc, char **argv);
	void set_simple_config();
};

#endif