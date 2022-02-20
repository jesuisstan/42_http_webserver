/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanfryd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 19:06:46 by ymanfryd          #+#    #+#             */
/*   Updated: 2022/02/20 19:06:53 by ymanfryd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/Response.hpp"

Response::Response(RequestParser &request): request_(request){
    std::cout << BgGREEN << request.getMethod() << request.getRoute() << RESET << std::endl;
    setResponseCodes();
    createResponse();
}

Response::Response(const Response &other) {
    *this = other;
}

Response &Response::operator=(const Response &other) {
    if (this != &other) {
        responseCodes_ = other.responseCodes_;
    }
    return *this;
}

void Response::setResponseCodes() {
    responseCodes_.insert(std::pair<int, std::string>(200, "200 OK\n"));
    responseCodes_.insert(std::pair<int, std::string>(400, "400 Bad Request\n"));
    responseCodes_.insert(std::pair<int, std::string>(404, "404 Not Found\n"));
    responseCodes_.insert(std::pair<int, std::string>(405, "405 Method Not Allowed\n"));
}

std::string Response::getResponse() {
    return std::string();
}

void Response::setResponse(std::string response) {
    response_ = response;
}

void Response::createResponse() {
    response_ = "HTTP/1.1";
    if (request_.getRoute() == "/")
        homeRoot();
}

void Response::homeRoot() {
    if (request_.getMethod() == "GET") {
        response_ += responseCodes_.find(200)->second;
        response_ += "Content-Type: text/html; charset UTF-8\nContent-Length: ";
        response_ += countContentLength();
    }
    else
        response_ += responseCodes_.find(405)->second;
}

std::string Response::countContentLength() {

    return std::string();
}
