#include "Location.hpp"

Location::Location(std::istream &ifs): root(""), cgi(""), autoindex(false)
{
	std::string cmnd;

	if (!(ifs >> cmnd) or cmnd != "{")
		baseError("Error start parcing location :" + cmnd);
	while ((ifs >> cmnd) and cmnd != "}")
	{
		if (cmnd == "methods")
			setMethods(ifs);
		else if (cmnd == "index")
			setIndex(ifs);
		else if (cmnd == "redirection")
			setRedirection(ifs);
		else if (cmnd == "root")
			setRoot(ifs);
		else if (cmnd == "cgi")
			setCgi(ifs);
		else if (cmnd == "autoindex")
			setAutoindex(ifs);
		else
			baseError("unrecognized keyWord in config: " + cmnd);
	}
	if (cmnd == "}")
	{
		if (methods.empty())
			methods.insert("GET");
		if (index.empty())
			index.insert(DEFAULT_INDEX);
		if (true) // todo ?
		{
			// std::cout << "got location\n" << this << std::endl;
			return;
		}
	}
	else
		baseError("Not found closing '}' for location");
}

void	Location::setMethods(std::istream &ifs) {
	std::stringstream	line;
	bool	found_semicolon;

	found_semicolon = false;

	if (!methods.empty())
		baseError("Dublicated methods string in location");
	std::getline(ifs, _cmnd);
	if (_cmnd == "")
		baseError("Error read methods");
	line << _cmnd;
	// std::cout << "methods line " << line.str() << std::endl;
	while ((line >> _cmnd) and not found_semicolon)
	{
		// std::cout << _cmnd << std::endl;
		if (_cmnd.size() and _cmnd[_cmnd.size() - 1] == ';')
		{
			_cmnd = cutSemicolon(_cmnd);
			found_semicolon = true;
		}
		if (methods.find(_cmnd) != methods.end())
			baseError("Dublicated methods in methods");
		methods.insert(_cmnd);
	}
	if (found_semicolon and not line.eof())
		baseError("Unprocessing parametrs after ';'"  + _cmnd);
	if (!found_semicolon)
		baseError("Failed parsing methods");
}

void	Location::setIndex(std::istream &ifs) {
	std::stringstream	line;
	bool	found_semicolon;

	found_semicolon = false;

	if (!index.empty())
		baseError("Dublicated index string in index");
	std::getline(ifs, _cmnd);
	if (_cmnd == "")
		baseError("Error read index");
	line << _cmnd;
	// std::cout << "index line " << line.str() << std::endl;
	while ((line >> _cmnd) and not found_semicolon)
	{
		// std::cout << _cmnd << std::endl;
		if (_cmnd.size() and _cmnd[_cmnd.size() - 1] == ';')
		{
			_cmnd = cutSemicolon(_cmnd);
			found_semicolon = true;
		}
		if (index.count(_cmnd))
			baseError("Dublicated index page");
		index.insert(_cmnd);
	}
	if (found_semicolon and not line.eof())
		baseError("Unprocessing parametrs after ';'"  + _cmnd);
	if (!found_semicolon)
		baseError("Failed parsing index");
}

void	Location::setRedirection(std::istream &ifs) {
	if (!(ifs >> redirection))
		baseError("Failed parsing redirection");
	redirection = cutSemicolon(redirection);
	// std::cout << "parced redirection " << redirection << std::endl;
}

void	Location::setRoot(std::istream &ifs) {
	if (!(ifs >> root))
		baseError("Failed parsing root");
	root = cutSemicolon(root);
	// std::cout << "parced root " << root << std::endl;
}

void	Location::setCgi(std::istream &ifs) {
	if (!(ifs >> cgi))
		baseError("Failed parsing cgi");
	cgi = cutSemicolon(cgi);
	// std::cout << "parced cgi " << cgi << std::endl;
}


void	Location::setAutoindex(std::istream &ifs) {
	std::string	line;

	if (!(ifs >> line))
		baseError("Failed parsing host");
	line = cutSemicolon(line);
	if (line != "on" and line != "off")
		baseError("Incorrect string in autoindex: " + line);
	autoindex = (line == "on");
}


// Location::Location(
// 	const std::set<std::string> &methods,
// 	const std::vector<std::string> &index,
// 	const std::string &root,
// 	const std::string &cgi,
// 	bool	autoindex ): 
// 		methods(methods), index(index), root(root), cgi(cgi), autoindex(autoindex)
// {
// }

const std::set<std::string> &Location::getMethods() const { return methods; }

const std::set<std::string> &Location::getIndex() const { return index; }

const std::string &Location::getRoot() const { return root; }

const std::string &Location::getRedirection() const { return redirection; }

const std::string &Location::getCgi() const { return cgi; }

bool Location::getAutoindex() const { return autoindex; }


std::ostream& operator<< (std::ostream &out, const Location &loca)
{
    out << "\t\tmethods: ";
	std::set<std::string> methods = loca.getMethods();
	std::copy(methods.begin(), methods.end(), std::ostream_iterator<std::string>(out, ", "));
	out << "\n\t\tindex: ";
	std::set<std::string> index = loca.getIndex();
	std::copy(index.begin(), index.end(), std::ostream_iterator<std::string>(out, ", "));
	out	<< "\n\t\troot: " << loca.getRoot() 
		<< "\n\t\tcgi: " << loca.getCgi()
		<< "\n\t\tautoindex: " << loca.getAutoindex()
		<< std::endl;
 
    return out;
}