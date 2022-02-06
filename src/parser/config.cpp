#include "config.hpp"

Config::Config(/* args */)
{
}

Config::~Config()
{
}

void get_config_from_file(int argc, char **argv)
{
    std::string config_file;
    
    if (argc > 1)
        config_file = argv[1];
    else
        config_file = DEFAULT_CONFIG_PATH;

}

