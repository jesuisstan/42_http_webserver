#include "webserv.hpp"
#include "Server.hpp"

int main(int argc, char *argv[]) {
	(void)argc;//todo
	(void)argv;//todo
	//if (argc != 2)
	//{
	//	std::cout << "Error: wrong amount of arguments!" << std::endl;
	//	exit(1);
	//}
	/* запускаем парсер конфига, получаем объект Config >>
	Config далее подаем аргументом в объект Server */
	// Server webserv(Config);
	Server webserv; //todo пока нет Config конструктор по-умолчанию
	webserv.initiate("127.0.0.1", PORT); // когда будет Config, метод сменится на .initiate(void)
	webserv.runServer(-1);

	return 0;
}
