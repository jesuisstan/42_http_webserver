#ifndef LOCATION_HPP
# define LOCATION_HPP

#include <vector>
#include <map>
#include <set>
#include <string>
#include <iostream>
#include <iterator>

class Location
{
private:
	std::set<std::string>		methods;
	std::vector<std::string>	index;
	std::string					root;
	std::string					cgi;
	bool						autoindex;
public:
	Location();
	Location(	const std::set<std::string> &methods,
				const std::vector<std::string> &index,
				const std::string &root,
				const std::string &cgi,
				bool	autoindex
				);

	const std::set<std::string> &getMethods() const;
	const std::vector<std::string> &getIndex() const;
	const std::string &getRoot() const;
	const std::string &getCgi() const;
	bool getAutoindex() const;
};

std::ostream& operator<< (std::ostream &out, const Location &loca);

#endif