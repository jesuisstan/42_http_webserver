#ifndef LOCATION_HPP
# define LOCATION_HPP

#include "webserv.hpp"

class Location
{
private:
	std::set<std::string>		methods;
	std::set<std::string>		index;
	std::string					alias;
	std::string					cgi;
	std::string					cgiExt;
	std::string					redirection;
	std::string					_cmnd;
	int							clientMaxBodySize;
	bool						autoindex;
public:

	Location() {};
	Location(std::istream &ifs);
	// Location(	const std::set<std::string> &methods,
	// 			const std::vector<std::string> &index,
	// 			const std::string &root,
	// 			const std::string &cgi,
	// 			bool	autoindex
	// 			);

	const std::set<std::string>		&getMethods() const;
	const std::set<std::string>		&getIndex() const;
	const std::string				&getRedirection() const;
	const std::string				&getAlias() const;
	const std::string				&getCgi() const;
	const std::string				&getCgiExt() const;
	const int						&getClientMaxBodySize() const;
	const bool						&getAutoindex() const;

	void							setMethods(std::istream &ifs);
	void							setIndex(std::istream &ifs);
	void							setRedirection(std::istream &ifs);
	void							setAlias(std::istream &ifs);
	void							setCgi(std::istream &ifs);
	void							setCgiExt(std::istream &ifs);
	void							setClientMaxBodySize(std::istream &ifs);
	void							setAutoindex(std::istream &ifs);
};



std::ostream& operator<< (std::ostream &out, const Location &loca);

#endif