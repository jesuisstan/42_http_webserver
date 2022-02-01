#include "webserv.hpp"

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
	char tmp[BUFFER_SIZE];
	for ( int i = 0 ; i < 75 ; i++)
		buffer[i] = str[i];

	std::cout << file << std::endl;
	int fd = open(file, O_RDONLY);
	fstat(fd, &buf);
	long indexSize = buf.st_size;
	fillLength(buffer, indexSize);
	read(fd, tmp, BUFFER_SIZE);

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
	int socket_fd = socket(AF_INET, SOCK_STREAM, 0);
	if (socket_fd == -1)
	{
		std::cout << "Failed to create socket with errno " << errno << std::endl;
		exit(EXIT_FAILURE);
	}
	sockaddr_in socketAddr = {};
	socketAddr.sin_family = AF_INET; 
	socketAddr.sin_addr.s_addr = INADDR_ANY;// todo или inet_addr("127.0.0.1");
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
		socklen_t addrlen = sizeof(sockaddr);
		int connection = accept(socket_fd, (struct sockaddr *)&socketAddr, &addrlen);
		if (connection < 0)
		{
			std::cout << "Failed to get data with errno " << errno << std::endl;
			exit(EXIT_FAILURE);
		}
		char buffer[BUFFER_SIZE];
		memset(buffer, 0, BUFFER_SIZE);
		ssize_t bytesRead = recv(connection, buffer, BUFFER_SIZE - 1, 0);
		buffer[bytesRead] = '\n';

		//std::string headers = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 47\n\n";
		//std::string body = "SURPRISE MOTHERF@CKER!\n\nCyberpunk ain't dead!!!\n";
		//std::string response = headers + body;

		memset(buffer, 0, BUFFER_SIZE);
		char *response = createResponse(buffer, "index.html");
		send(connection, response, ft_strlen(response), 0);
		close(connection);
	}
	close(socket_fd);
	return 0;
}
