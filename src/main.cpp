#include "../inc/webserv.hpp"
#include "Server.hpp"

int main()
{
	Server webserv("default_server");
	webserv.initiate("127.0.0.1", PORT);
	webserv.runServer(3 * 60 * 1000);
	webserv.closeConnections();

	return 0;
}
