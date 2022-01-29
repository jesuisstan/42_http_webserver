/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ErrorHandle.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanfryd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/01/29 15:10:37 by ymanfryd          #+#    #+#             */
/*   Updated: 2022/01/29 15:10:53 by ymanfryd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/ErrorHandle.hpp"

int Socket(int domain, int type, int protocol) {
    int res = socket(domain, type, protocol);
    if (res == -1) {
        std::cout << "Socket error: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    return res;
}

void Bind(int sockFd, const struct sockaddr *addr, socklen_t addrLen) {
    int res = bind(sockFd, addr, addrLen);
    if (res == -1) {
        std::cout << "Bind error: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

void Listen(int sockFd, int backLog) {
    int res = listen(sockFd, backLog);
    if (res == -1) {
        std::cout << "Listen error: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}


int Accept(int sockFd, struct sockaddr *addr, socklen_t *addrLen) {
    int res = accept(sockFd, addr, addrLen);
    if (res == -1) {
        std::cout << "Accept error: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    return res;
}

void Connect(int sockfd, const struct sockaddr *addr, socklen_t addrlen) {
    int res = connect(sockfd, addr, addrlen);
    if (res == -1) {
        std::cout << "Connect error: " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

void InetPton(int af, const char *src, void *dst) {
    int res = inet_pton(af, src, dst);
    if (res == 0) {
        std::cout << "InetPton error(0): " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
    if (res == -1) {
        std::cout << "InetPton error(1): " << errno << std::endl;
        exit(EXIT_FAILURE);
    }
}

char *Recv(int fd, char buffer[1024], size_t bufferSize, int flags) {
    ssize_t readSize = ::recv(fd, buffer, bufferSize, flags);
    if (readSize == -1) {
        std::cout << "Client read error: " << errno << std::endl;
        perror("qwe");
        exit(EXIT_FAILURE);
    } if (readSize == 0) {
        std::cout << "eof " << errno << std::endl;
    }
    return buffer;
}
