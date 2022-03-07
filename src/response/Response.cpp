/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.cpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: St.Krivtsov <1987stanislav@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 19:06:46 by ymanfryd          #+#    #+#             */
/*   Updated: 2022/02/26 17:23:54 by St.Krivtsov      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "Response.hpp"

Response::Response(RequestParser &request, ServerConfig &config) :
        requestRoute_(request.getRoute()),
        requestMethod_(request.getMethod()),
        responseCode_(0),
        contentLength_(0),
        requestPath_(request.getPath()),
        locations_(config.getLocations()),
        supportedMethods_(request.getSupportedMethods()),
        errorPages_(config.getErrorPages()) {
    setResponseCodes();
    createResponse();
}

Response::Response(const Response &other) {
    *this = other;
}

Response &Response::operator=(const Response &other) {
    if (this != &other) {
        request_ = other.request_;
        requestRoute_ = other.requestRoute_;
        requestMethod_ = other.requestMethod_;
        response_ = other.response_;
        responseCode_ = other.responseCode_;
        responseBody_ = other.responseBody_;
        responseHeaders_ = other.responseHeaders_;
        responseContentType_ = other.responseContentType_;
        responseCodes_ = other.responseCodes_;
        contentLength_ = other.contentLength_;
        locations_ = other.locations_;
        requestPath_ = other.requestPath_;
        supportedMethods_ = other.supportedMethods_;
        locationMethods_ = other.locationMethods_;
        locationIndex_ = other.locationIndex_;
        locationRoot_ = other.locationRoot_;
        requestedFile_ = other.requestedFile_;
        errorPages_ = other.errorPages_;
    }
    return *this;
}

/**************************/
/******** GETTERS *********/
/**************************/

const std::string &Response::getResponse() const {
    return this->response_;
}

const int &Response::getResponseCode() const {
    return this->responseCode_;
}

const size_t &Response::getContentLength() const {
    return this->contentLength_;
}

const std::string &Response::getResponseBody() const {
    return this->responseBody_;
}

const std::string &Response::getResponseHeaders() const {
    return this->responseHeaders_;
}

/**************************/
/******** SETTERS *********/
/**************************/

void Response::setResponse() {
    response_ = responseHeaders_ + "\n\r\n\r" + responseBody_;
}

void Response::setResponseCode(int code) {
    responseCode_ = code;
}

void Response::setContentType() {
    size_t routeEnd = requestPath_.size() - 1;
    size_t dotPos = requestPath_[routeEnd].find('.');
    if (dotPos != std::string::npos) {
        std::string extension = requestPath_[routeEnd].substr(dotPos + 1);
        if (extension == "css") {
            requestedFile_ = requestPath_[routeEnd];
            responseContentType_ = "text/css\n";
            trimRequestPath();
        } else if (extension == "js") {
            requestedFile_ = requestPath_[routeEnd];
            responseContentType_ = "application/javascript\n";
            trimRequestPath();
        }
    } else
        responseContentType_ = "text/html\n";
}

void Response::setResponseHeaders() {
//    std::cout << BgYELLOW << responseCode_ << RESET << std::endl;
    responseHeaders_ = "HTTP/1.1 ";
    responseHeaders_ += responseCodes_.find(responseCode_)->second;
    responseHeaders_ += "Content-Type: " + responseContentType_;
    responseHeaders_ += "charset UTF-8\nContent-Length: ";
    responseHeaders_ += std::to_string(contentLength_);
}

void Response::setResponseBody(const std::string &body) {
    responseBody_ = body;
}

void Response::setContentLength(size_t len) {
    contentLength_ = len;
}

void Response::setLocationMethods(const std::set<std::string> &locationMethods) {
    for (size_t j = 0; j < supportedMethods_.size(); j++) {
        if (locationMethods.count(supportedMethods_[j])) {
            locationMethods_.insert(supportedMethods_[j]);
        }
    }
}

void Response::setLocationIndex(const std::set<std::string> &locationIndex) {
    std::set<std::string>::iterator it;
    for (it = locationIndex.begin(); it != locationIndex.end(); it++) {
        locationIndex_.push_back(it->data());
    }
}

void Response::setLocationRoot(const std::string &locationRoot) {
    locationRoot_ = locationRoot;
}

void Response::setResponseCodes() {
    responseCodes_.insert(std::pair<int, std::string>(200, "200 OK\n"));
    responseCodes_.insert(std::pair<int, std::string>(400, "400 Bad Request\n"));
    responseCodes_.insert(std::pair<int, std::string>(404, "404 Not Found\n"));
    responseCodes_.insert(std::pair<int, std::string>(405, "405 Method Not Allowed\n"));
    responseCodes_.insert(std::pair<int, std::string>(500, "500 Internal Server Error\n"));
    responseCodes_.insert(std::pair<int, std::string>(502, "502 Bad Gateway\n"));
    responseCodes_.insert(std::pair<int, std::string>(503, "503 Service Unavailable\n"));
}

/**************************/
/******** HELPERS *********/
/**************************/


void Response::createResponse() {
    std::string body;

    setContentType();
    readLocationData();
//    std::cout << BgBLUE << errorPages_[404] << RESET << std::endl;
    bool locationSupportedMethod = locationMethods_.count(requestMethod_);
    if (locationMethods_.empty() && !requestedFile_.length())
        setResponseCode(404);
    else if (!locationSupportedMethod && !requestedFile_.length())
        setResponseCode(405);
    else
        setResponseCode(200);
    body = readContent(getScreen());
    setContentLength(body.size());
    setResponseHeaders();
    setResponseBody(body);
    setResponse();
}

std::string Response::findMaxPossibleLocation(const std::string& location) {
    std::string route = requestRoute_;

    for (size_t i = route.size(); i > 0; i--) {
        if (route == location)
            return route;
        route = route.substr(0, i);
    }
    return "";
}

void Response::readLocationData() {
    std::map<std::string, Location>::iterator iterator;
    std::string maxPossibleLocation;

    for (iterator = locations_.begin(); iterator != locations_.end(); iterator++) {
        if (!findMaxPossibleLocation(iterator->first).empty())
        maxPossibleLocation = findMaxPossibleLocation(iterator->first);
    }
    if (maxPossibleLocation.empty())
        maxPossibleLocation = "/";
    std::cout << BgRED << maxPossibleLocation << RESET << std::endl;
    for (iterator = locations_.begin(); iterator != locations_.end(); iterator++) {
        if (iterator->first == maxPossibleLocation) {
            setLocationRoot(iterator->second.getRoot());
            setLocationMethods(iterator->second.getMethods());
            setLocationIndex(iterator->second.getIndex());
        }

    }

}

std::string Response::readContent(const std::string &filename) {
    std::string buf;
    std::string content;
    std::ifstream inFile(filename);

    while (getline(inFile, buf))
        content += buf + "\n";
    inFile.close();
    return content;
}

std::string Response::getScreen() {
    std::string filename = locationRoot_;

    if (responseCode_ == 200 && !requestedFile_.length())
        filename += "/index.html";
    else if (responseCode_ == 200 && requestedFile_ == "style.css")
        filename += "/style.css";
    else if (responseCode_ == 200 && requestedFile_ == "index.js")
        filename += "/index.js";
    else if (errorPages_.find(responseCode_) != errorPages_.end())
        filename = "./errors/" + errorPages_[responseCode_];

    return filename;
}

void Response::trimRequestPath() {
    std::vector<std::string> newRequestPath;
    size_t routeEnd = requestPath_.size() - 1;
    if (routeEnd == 0) {
        newRequestPath.push_back("");
    }
    for (size_t i = 0; i < routeEnd; i++) {
        newRequestPath[i] = requestPath_[i];
    }
    requestPath_ = newRequestPath;
}
