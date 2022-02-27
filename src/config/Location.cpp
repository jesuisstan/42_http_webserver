#include "Location.hpp"

Location::Location(): root("/"), cgi(""), autoindex(false)
{

}

Location::Location(
	const std::set<std::string> &methods,
	const std::string &root,
	const std::string &cgi,
	bool	autoindex ): 
		methods(methods), root(root), cgi(cgi), autoindex(autoindex)
{

}

const std::set<std::string> &Location::getMethods() const {
	return methods;
}

const std::string &Location::getRoot() const {
	return root;
}

const std::string &Location::getCgi() const {
	return cgi;
}

bool Location::getAutoindex() const {
	return autoindex;
}

std::ostream& operator<< (std::ostream &out, const Location &loca)
{
    out << "\t\tmethods: ";
	std::set<std::string> methods = loca.getMethods();
	std::copy(methods.begin(), methods.end(), std::ostream_iterator<std::string>(out, ", "));
	out	<< "\n\t\troot: " << loca.getRoot() 
		<< "\n\t\tcgi: " << loca.getCgi()
		<< "\n\t\tautoindex: " << loca.getAutoindex()
		<< std::endl;
 
    return out;
}