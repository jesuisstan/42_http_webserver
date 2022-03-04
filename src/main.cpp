#include "webserv.hpp"
#include "Server.hpp"
#include "Config.hpp"

int main(int argc, char *argv[]) {
	(void)argc;//todo
	(void)argv;//todo
	std::cout << "C++ version is " << __cplusplus << std::endl << std::endl;
	//if (argc != 2)
	//{
	//	std::cout << "Error: wrong amount of arguments!" << std::endl;
	//	exit(1);
	//}
	/* запускаем парсер конфига, получаем объект Config >>
	Config далее подаем аргументом в объект Server */
	// Server webserv(Config);
	Server webserv; //todo пока нет Config конструктор по-умолчанию
	Config config(argc, argv);
	// config.set_config_with_3_servers(); // Тут сделаю замену на нормальное читание файла

	// std::map<std::string, Location> locs = sc.getLocations();
	// std::map<std::string, Location>::iterator it;
	// for (it=locs.begin(); it!=locs.end(); it++)
	// 	out << "\n\t" << it->first << '\n' << it->second;
	// std::map<int, std::vector<ServerConfig> >::iterator it;

	std::vector<ServerConfig> servers = config.getServers();
	for (size_t i = 1; i < servers.size(); i++)
	{
		std::cout << "Run server[" << i << "]\n" << servers[i];
		webserv.initiate(servers[i].getHost().c_str(), servers[i].getPort()); // когда будет Config, метод сменится на .initiate(void)
		webserv.runServer(-1);
	}
	// for (it=servers.begin(); it!=servers.end(); it++) // итерируется по портам
	// 	for (size_t i = 0; i < it->second.size(); i++) // итерируется по серверам на этом порте
	// 	{
	// 		webserv.initiate(it->second[i].getHost().c_str(), it->first); // когда будет Config, метод сменится на .initiate(void)
	// 		webserv.runServer(-1);
	// 	}




	return 0;
}
