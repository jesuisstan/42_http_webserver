#include "webserv.hpp"

int	main(void)
{
	while (42)
	{	
		int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
		if (socket_fd == -1)
		{
			std::cout << "Failed to create socket with errno " << errno << std::endl;
			exit(EXIT_FAILURE);
		}
		
		sockaddr_in serv_addr;
		serv_addr.sin_family = AF_INET; 
		serv_addr.sin_port = htons(PORT);
		inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr);
		
		socklen_t addrlen = sizeof(serv_addr);
		int connection = connect(socket_fd, (struct sockaddr *)&serv_addr, addrlen);
		if (connection < 0)
		{
			std::cout << "Failed to get data with errno " << errno << std::endl;
			exit(EXIT_FAILURE);
		}
		std::cout << "Client sends the following msg: ";
		std::string response;
		std::cin >> response;
		send(socket_fd, response.c_str(), response.size(), 0);
		response.clear();
	}
	return 0;
}
