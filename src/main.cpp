#include "../inc/webserv.hpp"

void fillLength(char *header, long size)
{
	int i = 0;

	while (header[i])
		i++;
	char *len = ft_itoa(size);
	size_t lenSize = ft_strlen(len);
	for (size_t j = 0; j <= lenSize; j++)
		header[i++] = len[j];
}

char *createResponse(char *buffer, const char *file)
{
	struct stat buf = {};
	const char *str = "HTTP/1.1 200 OK\\nContent-Type: text/html; charset UTF-8\\nContent-Length: 1778";
	char tmp[BUFFER_SIZE];
	for (int i = 0; i < 75; i++)
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
	while (buffer[i])
		i++;
	buffer[i++] = '\n';
	buffer[i] = '\n';
	while (tmp[j])
		buffer[i++] = tmp[j++];
	return buffer;
}

int g_server_onAir = true;

static void	close_connections(struct pollfd *fds, int number_fds)
{
	for (int i = 0; i < number_fds; i++)
	{
		if (fds[i].fd >= 0)
		{
			std::cout << "Connection " << fds[i].fd << " successfully closed" << std::endl;
			close(fds[i].fd);
		}
	}
}

static void	interruptHandler(int sig_int)
{
	(void)sig_int;
	std::cout << "\nAttention! Interruption signal caught\n";
	g_server_onAir = false;
}

int main()
{
	int rc, on = 1;
	int compress_array = false;

	int listen_sd = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sd < 0)
	{
		perror("socket() failed");
		exit(-1);
	}
	rc = setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on));
	if (rc < 0)
	{
		perror("setsockopt() failed");
		close(listen_sd);
		exit(-1);
	}

	/*	Set socket to be nonblocking */
	rc = fcntl(listen_sd, F_SETFL, fcntl(listen_sd, F_GETFL, 0) | O_NONBLOCK);
	if (rc < 0)
	{
		perror("fcntl() failed");
		close(listen_sd);
		exit(-1);
	}
	sockaddr_in addr = {};
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = INADDR_ANY; // todo или inet_addr("127.0.0.1");
	addr.sin_port = htons(PORT);
	rc = bind(listen_sd, (struct sockaddr *)&addr, sizeof(addr));
	if (rc < 0)
	{
		perror("bind() failed");
		close(listen_sd);
		exit(-1);
	}
	rc = listen(listen_sd, 32);
	if (rc < 0)
	{
		perror("listen() failed");
		close(listen_sd);
		exit(-1);
	}
	struct pollfd fds[BACKLOG] = {};
	fds[0].fd = listen_sd;
	fds[0].events = POLLIN;
	int timeout = 3 * 60 * 1000;

	/* Loop waiting for incoming connects or for incoming data */
	char buffer[BUFFER_SIZE];
	int nfds = 1;
	int current_size = 0;
	while (g_server_onAir != false)
	{
		signal(SIGINT, interruptHandler);
		std::cout << "Waiting on poll()...\n";
		rc = poll(fds, nfds, timeout);
		if (rc < 0)
		{
			perror("poll() failed");
			break;
		}
		if (rc == 0)
		{
			std::cout << "poll() timed out. End program.\n";
			break;
		}
		/* One or more descriptors are readable.
		Need to determine which ones they are*/
		current_size = nfds;
		for (int i = 0; i < current_size; i++)
		{
			/* Loop through to find the descriptors that returned	*/
			/* POLLIN and determine whether it's the listening	 */
			/* or the active connection */
			if (fds[i].revents == 0)
				continue;
			if (fds[i].revents && POLLIN)
			{
				if( fds[i].fd == listen_sd)
				{
					int new_sd = -1;
					do
					{
						/* Accept each incoming connection.
						If accept fails with EWOULDBLOCK, then we have accepted all of them.
						Any other failure on accept will cause us to end the server */
						int new_sd = accept(listen_sd, NULL, NULL);
						if (new_sd < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								perror("accept() failed");
								g_server_onAir = false;
							}
							break;
						}
						rc = fcntl(new_sd, F_SETFL, fcntl(new_sd, F_GETFL, 0) | O_NONBLOCK);
						if (rc < 0)
						{
							perror("fcntl() failed");
							close(listen_sd);
							exit(-1);
						}
						std::cout << "New incoming connection - " << new_sd << std::endl;
						fds[nfds].fd = new_sd;
						fds[nfds].events = POLLIN;
						nfds++;
					} while (new_sd != -1);
				}
				else
				{
					std::cout << fds[i].fd << " descriptor is readable" << std::endl;
					int close_conn = false;

					/* Receive all incoming data on this socket			*/
					/* before we loop back and call poll again.			*/
					while (true)
					{
						/* Receive data on this connection until the		 */
						/* recv fails with EWOULDBLOCK. If any other		 */
						/* failure occurs, we will close the connection.	 */
						int bytes_read = recv(fds[i].fd, buffer, sizeof(buffer), 0);
						if (bytes_read < 0)
						{
							if (errno != EWOULDBLOCK)
							{
								perror("recv() failed");
								close_conn = true;
							}
							break;
						}
						/* Check if the connection has been	closed by the client */
						if (bytes_read == 0)
						{
							close_conn = true;
							break;
						}
						std::cout << bytes_read << " bytes received" << std::endl;

						//std::string headers = "HTTP/1.1 200 OK\nContent-Type: text/plain\nContent-Length: 47\n\n";
						//std::string body = "SURPRISE MOTHERF@CKER!\n\nCyberpunk ain't dead!!!\n";
						//std::string resp = headers + body;
						//rc = send(fds[i].fd, resp.c_str(), resp.length(), 0);
						//std::cout << resp.length() << " - resp len\n";
						memset(buffer, 0, BUFFER_SIZE);
						char *response = createResponse(buffer, "index.html");
						rc = send(fds[i].fd, response, ft_strlen(response), 0);
						if (rc < 0)
						{
							perror("send() failed");
							close_conn = true;
							break;
						}
					}
					/* If the close_conn flag was turned on, we need	 */
					/* to clean up this active connection. This			*/
					/* clean up process includes removing the descriptor */
					if (close_conn)
					{
						close(fds[i].fd);
						std::cout << "Connection " << fds[i].fd << " closed\n";
						fds[i].fd = -1;
						compress_array = true;
					}
				}
			}
		}
		/* If the compress_array flag was turned on, we need
			to squeeze together the array and decrement the number
			of file descriptors */
		if (compress_array)
		{
			compress_array = false;
			for (int i = 0; i < nfds; i++)
			{
				if (fds[i].fd == -1)
				{
					for (int j = i; j < nfds; j++)
					{
						fds[j].fd = fds[j + 1].fd;
					}
					i--;
					nfds--;
				}
			}
		}
	}
	close_connections(fds, nfds);
}
