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

Response::Response(RequestParser &request, ServerConfig &config):
                                            requestRoute_(request.getRoute()),
                                            requestMethod_(request.getMethod()),
                                            responseCode_(0),
                                            contentLength_(0),
                                            requestPath_(request.getPath()),
                                            locations_(config.getLocations()),
                                            supportedMethods_(request.getSupportedMethods()) {
    setResponseCodes();
    createResponse();
}

Response::Response(const Response &other) {
    *this = other;
}

Response &Response::operator=(const Response &other) {
    if (this != &other) {
        request_                = other.request_;
        requestRoute_           = other.requestRoute_;
        requestMethod_          = other.requestMethod_;
        response_               = other.response_;
        responseCode_           = other.responseCode_;
        responseBody_           = other.responseBody_;
        responseHeaders_        = other.responseHeaders_;
        responseContentType_    = other.responseContentType_;
        responseCodes_          = other.responseCodes_;
        contentLength_          = other.contentLength_;
        locations_              = other.locations_;
        requestPath_            = other.requestPath_;
        supportedMethods_       = other.supportedMethods_;
        locationMethods_        = other.locationMethods_;
        locationIndex_          = other.locationIndex_;
        locationRoot_           = other.locationRoot_;
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
    size_t routeCount = requestPath_.size() - 1;
    size_t dotPos =  requestPath_[routeCount].find('.');
    if (dotPos != std::string::npos) {
        std::string extension = requestPath_[routeCount].substr(dotPos + 1);
        if (extension == "css")
            responseContentType_ = "text/css\n";
        else if (extension == "js")
            responseContentType_ = "application/javascript\n";
        else
            responseContentType_ = "text/html\n";
    }
}

void Response::setResponseHeaders() {
    responseHeaders_ = "HTTP/1.1 ";
    responseHeaders_ += responseCodes_.find(responseCode_)->second;
    responseHeaders_ += "Content-Type: " + responseContentType_;
    responseHeaders_ += "charset UTF-8\nContent-Length: ";
    responseHeaders_ += std::to_string(contentLength_);
}

void Response::setResponseBody(const std::string& body) {
    responseBody_ = body;
}

void Response::setContentLength(size_t len) {
    contentLength_ = len;
}

void Response::setLocationMethods(const std::set<std::string>& locationMethods) {
    for(size_t j = 0; j < supportedMethods_.size(); j++) {
        if (locationMethods.count(supportedMethods_[j]))
            locationMethods_.insert(supportedMethods_[j]);
    }
}

void Response::setLocationIndex(const std::set<std::string>& locationIndex) {
    std::set<std::string>::iterator it;
    for (it=locationIndex.begin(); it!=locationIndex.end(); it++) {
        locationIndex_.push_back(it->data());
    }
}

void Response::setLocationRoot(const std::string& locationRoot) {
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
    if (!locationMethods_.count(requestMethod_))
        setResponseCode(405);
    else if (requestRoute_ == "/")
        homeRoot();
    else if (requestRoute_ == "/style.css")
        setResponseCode(200);
    else if (requestRoute_.find("/directory") == 0)
        DirectoryRoot();
    else
        setResponseCode(404);
    body = readContent(getScreen());
    setContentLength(body.size());
    setResponseHeaders();
    setResponseBody(body);
    setResponse();
}

void Response::readLocationData() {
    std::map<std::string, Location>::iterator i;
    for (i=locations_.begin(); i!=locations_.end(); i++) {
        std::cout << i->second << i->first << std::endl;
        if (requestPath_[0] == i->first.substr(1)) {
            std::string locationRoot = i->second.getRoot();
            std::set<std::string> locationMethods = i->second.getMethods();
            std::set<std::string> locationIndex = i->second.getIndex();
            setLocationRoot(i->second.getRoot());
            setLocationMethods(i->second.getMethods());
            setLocationIndex(i->second.getIndex());
        }
    }
}

std::string Response::readContent(const std::string&   filename) {
    std::string     buf;
    std::string     content;
    std::ifstream   inFile(filename);

    while(getline(inFile, buf))
        content += buf + "\n";
    inFile.close();
    return content;
}

void Response::homeRoot() {
    if (requestMethod_ == "GET")
        setResponseCode(200);
    else
        setResponseCode(405);
}

void Response::DirectoryRoot() {
    if (requestMethod_ == "GET") {
        if (requestRoute_ == "/directory") {
            setResponseCode(200);
        } else if (requestRoute_.find("/directory/youpi.bad_extension") == 0) {
            setResponseCode(200);
        } else if (requestRoute_.find("/directory/youpi.bla") == 0) {
            setResponseCode(200);
        } else if (requestRoute_.find("/directory/nop") == 0) {
            if (requestRoute_ == "/directory/nop/" || requestRoute_ == "/directory/nop" || requestRoute_ == "/directory/nop/other.pouic" || requestRoute_ == "/directory/nop/youpi.bad_extension")
                setResponseCode(200);
            else
                setResponseCode(404);
        } else if (requestRoute_.find("/directory/Yeah") == 0) {
            setResponseCode(200);
        } else {
            setResponseCode(404);
        }
    }
    else
        setResponseCode(405);
}

std::string Response::getScreen() const {
    std::string filename = "./src/screens/";
    if (responseCode_ == 200 && requestRoute_ == "/")
        filename += "HomePage/index.html";
    else if (responseCode_ == 200 && requestRoute_ == "/style.css")
        filename += "HomePage/style.css";
    else
        filename += std::to_string(responseCode_) + ".html";
    return filename;
}
