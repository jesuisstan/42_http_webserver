#include "webserv.hpp"

int	main(void)
{
	/* create a socket 
	int socket(int domain, int type, int protocol);
	int domain: AF_INET - IPv4 Internet protocols
	int type: SOCK_STREAM - Two-way reliable communication (TCP)
	int protocol: Normally there is only 1 protocol for each type, so the value 0
	*/
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		std::cout << "Failed to create socket with errno " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	sockaddr_in socketAddr;
	socketAddr.sin_family = AF_INET; 
	socketAddr.sin_addr.s_addr = inet_addr("10.21.21.74"); //INADDR_ANY;// todo или inet_addr("127.0.0.1");
	socketAddr.sin_port = htons(PORT);
	if (bind(socket_fd, (struct sockaddr *)&socketAddr, sizeof(socketAddr)) < 0)
	{
		std::cout << RED << "Failed to bind to port with errno " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	if (listen(socket_fd, BACKLOG) < 0)
	{
		std::cout << RED << "Failed to listen on socket with errno " << errno << std::endl;
		exit(EXIT_FAILURE);
	}

	while (true)
	{
		/* start accepting requests
		accept extracts an element from a queue of connections
			(The queue created by listen) for a socket.
		int accept(int sockfd, sockaddr *addr, socklen_t *addrlen);
		! addrlen is now a value-result argument - needs pointer
		*/
		socklen_t addrlen = sizeof(sockaddr);
		int connection = accept(socket_fd, (struct sockaddr *)&socketAddr, &addrlen);
		if (connection < 0)
		{
			std::cout << "Failed to get data with errno " << errno << std::endl;
			exit(EXIT_FAILURE);
		}
		
		/* Read from the connection
		*/
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, strlen(buffer));
		//int bytesRead = read(connection, buffer, BUFFER_SIZE);
		int bytesRead = recv(connection, buffer, BUFFER_SIZE - 1, 0);
		buffer[bytesRead] = '\n';
		std::cout << buffer;

		// Send a message to the connection
		std::string headers = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 47\n\n";
		std::string body = "SURPRISE MOTHERF@CKER!\n\nCyberpunk ain't dead!!!\n";
		std::string response = headers + body;
		//std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 47\n\nSURPRISE MOTHERF@CKER!\nCyberpunk ain't dead!!!\n";
		std::cout << response;
		send(connection, response.c_str(), response.size(), 0);
		close(connection);
	}
	// Clean up!
	close(socket_fd);
	return 0;
}

