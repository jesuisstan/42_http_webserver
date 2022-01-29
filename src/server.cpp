/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   server.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: St.Krivtsov <1987stanislav@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/28 21:26:32 by St.Krivtsov       #+#    #+#             */
/*   Updated: 2022/01/29 01:40:31 by St.Krivtsov      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "../inc/webserv.hpp"

void	fillLength(char *header, long size)
{
    int		i = 0;

    while (header[i])
        i++;
    char *len = ft_itoa(size);
    size_t lenSize = ft_strlen(len);
    for (size_t j = 0; j <= lenSize; j++ )
        header[i++] = len[j];
}

char *createResponse(char *buffer, const char *file) {
    struct stat	buf = {};
    const char *str = "HTTP/1.1 200 OK\\nContent-Type: text/html; charset UTF-8\\nContent-Length: ";
    char tmp[10000];
    for ( int i = 0 ; i < 75 ; i++)
        buffer[i] = str[i];

    std::cout << file << std::endl;
    int fd = open(file, O_RDONLY);
    fstat(fd, &buf);
    long indexSize = buf.st_size;
    fillLength(buffer, indexSize);
    read(fd, tmp, 10000);

    close(fd);
    int j = 0;
    int i = 0;
    while(buffer[i])
        i++;
    buffer[i++] = '\n';
    buffer[i] = '\n';
    while (tmp[j])
        buffer[i++] = tmp[j++];
    return buffer;
}

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
	sockaddr_in socketAddr = {};
	socketAddr.sin_family = AF_INET; 
	socketAddr.sin_addr.s_addr = INADDR_ANY;// todo или inet_addr("127.0.0.1");
	socketAddr.sin_port = htons(8080);
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
		char buffer[10024];
        ft_memset(buffer, 0, 10024);
        ssize_t bytesRead = ::recv(connection, buffer, 10024, 0);
		buffer[bytesRead] = '\n';
		std::cout << buffer;

        ft_memset(buffer, 0, 10024);
        char *response = createResponse(buffer, "index.html");
        std::cout << buffer;
        // Send a message to the connection
        send(connection, response, ft_strlen(response), 0);
		close(connection);
	}
	// Clean up!
	close(socket_fd);
	return 0;
}