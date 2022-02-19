//
// Created by Yellowjacket Manfryd on 2/19/22.
//

#include "../inc/RequestParser.hpp"

RequestParser::RequestParser() {
}

RequestParser::RequestParser(std::string request):request_(request) {
//    std::cout << "____PARSER CONSTRUCTED____" << std::endl;
//    std::cout << request_ << std::endl;
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
        setUpgradeInsecureRequests();
        setSecFetchDest();
        setSecFetchMode();
        setSecFetchSite();
        setSecFetchUser();
        setCacheControl();
        setBody();
    }

//    std::cout << "____PARSER CONSTRUCTOR END____" << std::endl;
}

RequestParser::RequestParser(const RequestParser &other) {
    *this = other;
}

RequestParser &RequestParser::operator=(const RequestParser &other) {
    if (this != &other) {
        request_ = other.request_;
        method_ = other.method_;
        protocol_ = other.protocol_;
        host_ = other.host_;
        userAgent_ = other.userAgent_;
        accept_ = other.accept_;
        acceptLanguage_ = other.acceptLanguage_;
        acceptEncoding_ = other.acceptEncoding_;
        connection_ = other.connection_;
        upgradeInsecureRequests_ = other.upgradeInsecureRequests_;
        secFetchDest_ = other.secFetchDest_;
        secFetchMode_ = other.secFetchMode_;
        secFetchSite_ = other.secFetchSite_;
        secFetchUser_ = other.secFetchUser_;
        cacheControl_ = other.cacheControl_;
        body_ = other.body_;
    }
    return *this;
}

RequestParser::~RequestParser() {
//    std::cout << "____PARSER DESTRUCTED____" << std::endl;
}

/**************************/
/******** GETTERS *********/
/**************************/

std::string RequestParser::getRequest() {
    return this->request_;
}

std::string RequestParser::getMethod() {
    return this->method_;
}

std::string RequestParser::getRoute() {
    return this->route_;
}

std::string RequestParser::getProtocol() {
    return this->protocol_;
}


std::string RequestParser::getHost() {
    return this->host_;
}

std::string RequestParser::getUserAgent() {
    return this->userAgent_;
}

std::string RequestParser::getAccept() {
    return this->accept_;
}

std::string RequestParser::getAcceptLanguage() {
    return this->acceptLanguage_;
}

std::string RequestParser::getAcceptEncoding() {
    return this->acceptEncoding_;
}

std::string RequestParser::getConnection() {
    return this->connection_;
}

std::string RequestParser::getUpgradeInsecureRequests() {
    return this->upgradeInsecureRequests_;
}

std::string RequestParser::getSecFetchDest() {
    return this->secFetchDest_;
}

std::string RequestParser::getSecFetchMode() {
    return this->secFetchMode_;
}

std::string RequestParser::getSecFetchSite() {
    return this->secFetchSite_;
}

std::string RequestParser::getSecFetchUser() {
    return this->secFetchUser_;
}

std::string RequestParser::getCacheControl() {
    return this->cacheControl_;
}

std::string RequestParser::getBody() {
    return this->body_;
}

/**************************/
/******** SETTERS *********/
/**************************/


void RequestParser::setMethod() {
    method_ = parseByChar(request_, ' ');
//    std::cout << "__METHOD_____________|" << method_ << "|" << std::endl;
}

void RequestParser::setRoute() {
    std::string fistReqStr = request_.substr(0, request_.find_first_of('\n') - 1);
    int routeStart = fistReqStr.find_first_of(' ') + 1;
    int routeEnd = fistReqStr.find_last_of(' ') - 1;
    route_ = fistReqStr.substr(routeStart, routeEnd - 3);
//    std::cout << "__ROUTE______________|" <<  route_ << "|" << std::endl;
}

void RequestParser::setProtocol() {
    std::string fistReqStr = request_.substr(0, request_.find_first_of('\n') - 1);
    int protocolStart = fistReqStr.find_last_of(' ') + 1;
    int protocolEnd = fistReqStr.find_last_of('/') - 1;
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

void RequestParser::setUpgradeInsecureRequests() {
    upgradeInsecureRequests_ = parseByHeaderName("Upgrade-Insecure-Requests:");
//    std::cout << "__UPGRADE_INS________|" << upgradeInsecureRequests_ << "|" << std::endl;
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

}


void RequestParser::setSupportedMethods() {
    supportedMethods_.push_back("GET");
    supportedMethods_.push_back("POST");
    supportedMethods_.push_back("DELETE");
}

/**************************/
/******** HELPERS *********/
/**************************/

bool RequestParser::isSupportedMethod() {
    for (size_t i = 0; i < supportedMethods_.size(); i++) {
        if (method_.compare(supportedMethods_[i]) == 0)
            return true;
    }
    return false;
}

std::string RequestParser::parseByChar(std::string string, char symbol) {
    iterator_ = string.find_first_of(symbol);
    if (request_[iterator_ - 1] == ' ')
        return request_.substr(0, iterator_ - 1);
    else
        return request_.substr(0, iterator_ );
}
std::string RequestParser::parseByHeaderName(std::string name) {
    std::string erasedRequest;
    iterator_ = request_.find(name);
    iterator_ += name.size();
    if (request_[iterator_ + 1] == ' ')
        erasedRequest = request_.erase(0, iterator_);
    else
        erasedRequest = request_.erase(0, iterator_ + 1);
    return erasedRequest.substr(0, erasedRequest.find_first_of('\n') - 1) ;
}

std::string RequestParser::eraseRequest() {
    if (request_[iterator_ + 1] == ' ')
        return request_.erase(0, iterator_ + 2);
    else
        return request_.erase(0, iterator_ + 1);
}




