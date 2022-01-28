/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: St.Krivtsov <1987stanislav@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 21:26:32 by St.Krivtsov       #+#    #+#             */
/*   Updated: 2022/01/28 23:34:50 by St.Krivtsov      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/webserv.hpp"

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
	/* use bind to assign an IP address and port to the socket
	int bind(int sockfd, const sockaddr *addr, socklen_t addrlen);
	*/
	sockaddr_in socketAddr;
	socketAddr.sin_family = AF_INET; 
	socketAddr.sin_addr.s_addr = INADDR_ANY;
	socketAddr.sin_port = htons(8088);
	if (bind(socket_fd, (struct sockaddr *)&socketAddr, sizeof(socketAddr)) < 0)
	{
		std::cout << "Failed to bind to port 8080 with errno " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	
	/* start listening to socket_fd
	listen marks a socket as passive (socket will be used to accept connections)
	int listen(int sockfd, int backlog);
	*/
	if (listen(socket_fd, BACKLOG) < 0)
	{
		std::cout << "Failed to listen on socket with errno " << errno << std::endl;
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
		char buffer[100];
		read(connection, buffer, 100);
		std::cout << buffer;

		// Send a message to the connection
		std::string response = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 47\n\nSURPRISE MOTHERF@CKER!\nCyberpunk ain't dead!!!\n";
		send(connection, response.c_str(), response.size(), 0);
		close(connection);
	}
	// Clean up!
	close(socket_fd);
	return 0;
}