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
	Config config;
	config.set_simple_config(); // Тут сделаю замену на нормальное читание файла

	// std::map<std::string, Location> locs = sc.getLocations();
	// std::map<std::string, Location>::iterator it;
	// for (it=locs.begin(); it!=locs.end(); it++)
	// 	out << "\n\t" << it->first << '\n' << it->second;

	std::map<int, std::vector<ServerConfig> > servers = config.getServers();
	std::map<int, std::vector<ServerConfig> >::iterator it;
	for (it=servers.begin(); it!=servers.end(); it++) // итерируется по портам
		for (size_t i = 0; i < it->second.size(); i++) // итерируется по серверам на этом порте
		{
			webserv.initiate(it->second[i].getHost().c_str(), it->first); // когда будет Config, метод сменится на .initiate(void)
			webserv.runServer(-1);
		}




	return 0;
}
