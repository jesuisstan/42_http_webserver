/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Client.cpp                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanfryd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 15:00:15 by ymanfryd          #+#    #+#             */
/*   Updated: 2022/01/29 15:00:19 by ymanfryd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ErrorHandle.hpp"

int main () {
    int socket_fd = Socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in adr = {0};
    adr.sin_family = AF_INET;
    adr.sin_port = htons(8088);
    InetPton(AF_INET, "127.0.0.1", &adr.sin_addr );
    Connect(socket_fd, (struct sockaddr *) &adr, sizeof adr);
    char buffer[1024];
    read(0, buffer, 1024);
    write(socket_fd, buffer, 1024);
    char *buf;
    char buf2[1024];
    ssize_t nread = read(socket_fd, buf2, 1024);
    write(STDOUT_FILENO, buf, nread);
    write(socket_fd, buf, nread);
    close(socket_fd);
    return 0;
}