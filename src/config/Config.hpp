#ifndef CONFIG_HPP
# define CONFIG_HPP

#include "webserv.hpp"
#include "ServerConfig.hpp"

#define DEFAULT_CONFIG_PATH "configs/default.cfg"

class Config
{
private:
	std::set<std::string>	portHosts;
	std::vector<ServerConfig>	servers;

public:
	Config(/* args */);
	Config(char *filename);
	Config(int argc, char **argv);
	~Config();
	const std::vector<ServerConfig> &getServers() const;
};

#endif
