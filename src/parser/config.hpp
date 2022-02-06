#ifndef CONFIG_HPP
#define CONFIG_HPP

#include <vector>
#include <map>
#include <set>

#define DEFAULT_CONFIG_PATH "default.cfg"

class Config
{
private:
    std::set<int> ports;
    std::map<int, std::vector<ServerConfig>> servers;

public:
    Config(/* args */);
    ~Config();
    void get_config_from_file(int argc, char **argv)
};

Config::Config(/* args */)
{
}

Config::~Config()
{
}

#endif