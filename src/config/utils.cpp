#include "webserv.hpp"

void	baseError(std::string errorText)
{
	std::cerr << errorText << std::endl;
	exit(1);
};

// template <typename T>
int		stringToNumber (std::string &text)
{
	int number;
	if ( ! (std::istringstream(text) >> number) )
		number = 0;
	return number;
}

bool	isPositiveDigit(std::string &s)
{
	for (size_t i = 0; i < s.size(); i++)
		if (!std::isdigit(s[i]))
			return false;
	return true;
}

int hexToDec(std::string hex)
{
    int len = hex.size();
    int base = 1;
    int dec = 0;
    for (int i = len - 1; i >= 0; i--) {
        if (hex[i] >= '0' && hex[i] <= '9') {
            dec += (int(hex[i]) - 48) * base;
            base = base * 16;
        }
        else if (hex[i] >= 'A' && hex[i] <= 'F') {
            dec += (int(hex[i]) - 55) * base;
            base = base * 16;
        }
        else if (hex[i] >= 'a' && hex[i] <= 'f') {
            dec += (int(hex[i]) - 87) * base;
            base = base * 16;
        }
    }
    return dec;
}


void	readSemicolon(std::istream &ifs)
{
	std::string	cmnd;

	if (!(ifs >> cmnd) or cmnd != ";")
		baseError("Not found semicolon");
}

std::string &cutSemicolon(std::string &line)
{
	if (line.size() and line[line.size() - 1] == ';')
		line.resize(line.size() - 1);
	else
		baseError("Not found semicolon in the end of the line: " + line);
	return line;
}