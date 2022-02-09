#ifndef SERVERCONFIG_HPP
#define SERVERCONFIG_HPP

#include <vector>
#include <map>
#include <set>

#define DEFAULT_ERROR_PAGE "error.html"
#define DEFAULT_PORT 8080

typedef struct s_location
{
	std::set<std::string> methods;
	std::string root = NULL;
	std::string icg = NULL;
	bool autoindex = true;
} t_locations;


typedef struct s_server_config
{
	std::string host;
	int port = DEFAULT_PORT;
	std::string error_page = DEFAULT_ERROR_PAGE;
	std::map<std::string, std::vector<s_location> > locs;
} t_server_config;

#endif