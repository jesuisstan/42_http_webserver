/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.cpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: St.Krivtsov <1987stanislav@gmail.com>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 19:06:27 by ymanfryd          #+#    #+#             */
/*   Updated: 2022/02/26 17:23:49 by St.Krivtsov      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "RequestParser.hpp"

RequestParser::RequestParser() {
}

RequestParser::RequestParser(std::string request, size_t requestLen) {
    request_ = request.substr(0, requestLen);
    parse();
}

RequestParser::RequestParser(const RequestParser &other) {
    *this = other;
}

RequestParser &RequestParser::operator=(const RequestParser &other) {
    if (this != &other) {
        request_        = other.request_;
        method_         = other.method_;
        protocol_       = other.protocol_;
        host_           = other.host_;
        userAgent_      = other.userAgent_;
        accept_         = other.accept_;
        acceptLanguage_ = other.acceptLanguage_;
        acceptEncoding_ = other.acceptEncoding_;
        connection_     = other.connection_;
        secFetchDest_   = other.secFetchDest_;
        secFetchMode_   = other.secFetchMode_;
        secFetchSite_   = other.secFetchSite_;
        secFetchUser_   = other.secFetchUser_;
        cacheControl_   = other.cacheControl_;
        headers_        = other.headers_;
        contentLength_  = other.contentLength_;
        body_           = other.body_;
    }
    return *this;
}

/**************************/
/******** GETTERS *********/
/**************************/

std::string RequestParser::getRequest() const {
    return this->request_;
}

std::string RequestParser::getMethod() const {
    return this->method_;
}

std::string RequestParser::getRoute() const {
    return this->route_;
}

std::string RequestParser::getProtocol() const {
    return this->protocol_;
}


std::string RequestParser::getHost() const {
    return this->host_;
}

std::string RequestParser::getUserAgent() const {
    return this->userAgent_;
}

std::string RequestParser::getAccept() const {
    return this->accept_;
}

std::string RequestParser::getAcceptLanguage() const {
    return this->acceptLanguage_;
}

std::string RequestParser::getAcceptEncoding() const {
    return this->acceptEncoding_;
}

std::string RequestParser::getConnection() const {
    return this->connection_;
}

std::string RequestParser::getSecFetchDest() const {
    return this->secFetchDest_;
}

std::string RequestParser::getSecFetchMode() const {
    return this->secFetchMode_;
}

std::string RequestParser::getSecFetchSite() const {
    return this->secFetchSite_;
}

std::string RequestParser::getSecFetchUser() const {
    return this->secFetchUser_;
}

std::string RequestParser::getCacheControl() const {
    return this->cacheControl_;
}

std::string RequestParser::getBody() const {
    return this->body_;
}

std::string RequestParser::getHeaders() const {
    return this->headers_;
}

size_t RequestParser::getContentLength() const {
    return this->contentLength_;
}

/**************************/
/******** SETTERS *********/
/**************************/


void RequestParser::setMethod() {
    method_ = parseByChar(request_, ' ');
//    std::cout << "__METHOD_____________|" << method_ << "|" << std::endl;
}

void RequestParser::setRoute() {
    std::string fistReqStr = request_;
    fistReqStr = fistReqStr.substr(0, request_.find_first_of('\n') - 1);
    size_t routeStart = fistReqStr.find_first_of(' ') + 1;
    size_t routeEnd = fistReqStr.find_last_of(' ') - 1;
    if (routeStart != routeEnd)
        route_ = EraseSpaces(fistReqStr.substr(routeStart, routeEnd));
    else
        route_ = "/";
}

void RequestParser::setProtocol() {
    std::string fistReqStr = request_;
    fistReqStr = fistReqStr.substr(0, request_.find_first_of('\n') - 1);
    size_t protocolStart = fistReqStr.find_last_of(' ') + 1;
    size_t protocolEnd = fistReqStr.find_last_of('/') - 1;
    protocol_ = fistReqStr.substr(protocolStart, protocolEnd);
//    std::cout << "__PROTOCOL___________|" <<  protocol_ << "|" << std::endl;
}

void RequestParser::setHost() {
    host_ = parseByHeaderName("Host:");
//    std::cout << "__HOST_______________|" << host_ << "|" << std::endl;
}

void RequestParser::setUserAgent() {
    userAgent_ = parseByHeaderName("User-Agent:");
//    std::cout << "__USER_AGENT_________|" << userAgent_ << "|" << std::endl;
}

void RequestParser::setAccept() {
    accept_ = parseByHeaderName("Accept:");
//    std::cout << "__ACCEPT_____________|" << accept_ << "|" << std::endl;
}

void RequestParser::setAcceptLanguage() {
    acceptLanguage_ = parseByHeaderName("Accept-Language:");
//    std::cout << "__ACCEPT_LANGUAGE____|" << acceptLanguage_ << "|" << std::endl;
}

void RequestParser::setAcceptEncoding() {
    acceptEncoding_ = parseByHeaderName("Accept-Encoding:");
//    std::cout << "__ACCEPT_ENCODING____|" << acceptEncoding_ << "|" << std::endl;
}

void RequestParser::setConnection() {
    connection_ = parseByHeaderName("Connection:");
//    std::cout << "__CONNECTION_________|" << connection_ << "|" << std::endl;
}

void RequestParser::setSecFetchDest() {
    secFetchDest_ = parseByHeaderName("Sec-Fetch-Dest:");
//    std::cout << "__SEC_FETCH_DEST_____|" << secFetchDest_ << "|" << std::endl;
}

void RequestParser::setSecFetchMode() {
    secFetchMode_ = parseByHeaderName("Sec-Fetch-Mode:");
//    std::cout << "__SEC_FETCH_MODE_____|" << secFetchMode_ << "|" << std::endl;
}

void RequestParser::setSecFetchSite() {
    secFetchSite_ = parseByHeaderName("Sec-Fetch-Site:");
//    std::cout << "__SEC_FETCH_SITE_____|" << secFetchSite_ << "|" << std::endl;
}

void RequestParser::setSecFetchUser() {
    secFetchUser_ = parseByHeaderName("Sec-Fetch-User:");
//    std::cout << "__SEC_FETCH_USER_____|" << secFetchUser_ << "|" << std::endl;
}

void RequestParser::setCacheControl() {
    cacheControl_ = parseByHeaderName("Cache-Control:");
//    std::cout << "__CACHE_CONTROL______|" << cacheControl_ << "|" << std::endl;
}

void RequestParser::setBody() {
    std::string erasedRequest = request_;
    size_t bodyStart = request_.find("\n\r");
    if (bodyStart != std::string::npos)
        body_ = erasedRequest.substr(bodyStart + 3);
//    std::cout << "__BODY_______________|" << body_ << "|" << std::endl;
}

void        RequestParser::setHeaders() {
    std::string erasedRequest = request_;
    size_t headersEnd = request_.find("\n\r");
    if (headersEnd != std::string::npos)
        headers_ = erasedRequest.substr( 0,headersEnd);
//    std::cout << "__HEADERS____________|" << GREEN << headers_  << RESET << std::endl;
}

void        RequestParser::setContentLength() {
    std::string contentLengthStr = parseByHeaderName("Content-Length:");
    try {
        if (contentLengthStr.length())
            contentLength_ = std::stol(contentLengthStr); // это что-то из 11 стандарта, c флагами не скомпилится
        else
            contentLength_ = 0;
    } catch (std::exception &exception) {
        std::cout << exception.what() << std::endl;
    }
//    std::cout << "__CONTENT_LENGTH_____|" << contentLength_ << "|" << std::endl;
}

void RequestParser::setSupportedMethods() {
    supportedMethods_.push_back("GET");
    supportedMethods_.push_back("POST");
    supportedMethods_.push_back("DELETE");
    supportedMethods_.push_back("HEAD");
    supportedMethods_.push_back("PUT");
}


/**************************/
/******** HELPERS *********/
/**************************/


void RequestParser::parse() {
    setSupportedMethods();
    setMethod();
    if (isSupportedMethod()) {
        setRoute();
        setProtocol();
        setHost();
        setUserAgent();
        setAccept();
        setAcceptLanguage();
        setAcceptEncoding();
        setConnection();
        setSecFetchDest();
        setSecFetchMode();
        setSecFetchSite();
        setSecFetchUser();
        setCacheControl();
        setBody();
        setHeaders();
        setContentLength();
    } else {
        throw UnsupportedMethodException(method_);
    }
}


bool RequestParser::isSupportedMethod() {
    for (size_t i = 0; i < supportedMethods_.size(); i++) {
        if (method_ == supportedMethods_[i])
            return true;
    }
    return false;
}

std::string RequestParser::parseByChar(const std::string& string, char symbol) {
    std::string erasedRequest = request_;
    iterator_ = string.find_first_of(symbol);
    if (iterator_ == std::string::npos)
        return "";
    if (request_[iterator_ - 1] == ' ')
        return erasedRequest.substr(0, iterator_ - 1);
    return erasedRequest.substr(0, iterator_ );
}
std::string RequestParser::parseByHeaderName(const std::string& name) {
    std::string erasedRequest = request_;
    iterator_ = request_.find(name);
    if (iterator_ == std::string::npos)
        return "";
    iterator_ += name.size();
    if (request_[iterator_ + 1] == ' ')
        erasedRequest = erasedRequest.erase(0, iterator_);
    else
        erasedRequest = erasedRequest.erase(0, iterator_ + 1);
    return erasedRequest.substr(0, erasedRequest.find_first_of('\n') - 1) ;
}

std::string RequestParser::EraseSpaces(const std::string& string) {
    size_t spacePos = string.find(' ');
    std::string result = string;
    while (spacePos != std::string::npos) {
        result = result.replace(spacePos,spacePos, "");
        spacePos = result.find(' ');
    }
    return result;
}


RequestParser::UnsupportedMethodException::UnsupportedMethodException(const std::string &method) {
    message_ = RED"Method " + method + " is not supported" RESET;
}

const char* RequestParser::UnsupportedMethodException::what() const throw() {
    return (message_.c_str());
}
