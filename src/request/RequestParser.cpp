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
        host_             = other.host_;
        body_             = other.body_;
        route_            = other.route_;
        query_            = other.query_;
        accept_           = other.accept_;
        method_           = other.method_;
        headers_          = other.headers_;
        request_          = other.request_;
        pathInfo_         = other.pathInfo_;
        protocol_         = other.protocol_;
        userAgent_        = other.userAgent_;
        connection_       = other.connection_;
        contentType_      = other.contentType_;
        secFetchDest_     = other.secFetchDest_;
        secFetchMode_     = other.secFetchMode_;
        secFetchSite_     = other.secFetchSite_;
        secFetchUser_     = other.secFetchUser_;
        cacheControl_     = other.cacheControl_;
        acceptLanguage_   = other.acceptLanguage_;
        acceptEncoding_   = other.acceptEncoding_;
        pathTranslated_   = other.pathTranslated_;
        isChunked_        = other.isChunked_;

        path_             = other.path_;
        iterator_         = other.iterator_;
        contentLength_    = other.contentLength_;
        supportedMethods_ = other.supportedMethods_;
    }
    return *this;
}

/**************************/
/******** GETTERS *********/
/**************************/

const std::string &RequestParser::getRequest() const {
    return this->request_;
}

const std::string &RequestParser::getMethod() const {
    return this->method_;
}

const std::string &RequestParser::getRoute() const {
    return this->route_;
}

const std::string &RequestParser::getQuery() const {
    return this->query_;
}

const std::string &RequestParser::getProtocol() const {
    return this->protocol_;
}


const std::string &RequestParser::getHost() const {
    return this->host_;
}

const std::string &RequestParser::getUserAgent() const {
    return this->userAgent_;
}

const std::string &RequestParser::getAccept() const {
    return this->accept_;
}

const std::string &RequestParser::getAcceptLanguage() const {
    return this->acceptLanguage_;
}

const std::string &RequestParser::getAcceptEncoding() const {
    return this->acceptEncoding_;
}

const std::string &RequestParser::getConnection() const {
    return this->connection_;
}

const std::string &RequestParser::getSecFetchDest() const {
    return this->secFetchDest_;
}

const std::string &RequestParser::getSecFetchMode() const {
    return this->secFetchMode_;
}

const std::string &RequestParser::getSecFetchSite() const {
    return this->secFetchSite_;
}

const std::string &RequestParser::getSecFetchUser() const {
    return this->secFetchUser_;
}

const std::string &RequestParser::getCacheControl() const {
    return this->cacheControl_;
}

const std::string &RequestParser::getBody() const {
    return this->body_;
}

const std::map<std::string, std::string> &RequestParser::getHeaders() const {
    return this->headers_;
}

const size_t &RequestParser::getContentLength() const {
    return this->contentLength_;
}

const std::string &RequestParser::getContentType() const {
    return this->contentType_;
}

const std::string &RequestParser::getPathInfo() const {
    return this->pathInfo_;
}

const std::vector <std::string> &RequestParser::getPath() const {
    return this->path_;
}

const std::vector <std::string> &RequestParser::getSupportedMethods() const {
    return this->supportedMethods_;
}

const std::string &RequestParser::getPathTranslated() const {
    return this->pathTranslated_;
}

/**************************/
/******** SETTERS *********/
/**************************/


void RequestParser::setMethod() {
    method_ = parseByChar(request_, ' ');
//    std::cerr << "__METHOD_____________|" << method_ << "|" << std::endl;
}

void RequestParser::setRoute() {
    std::string fistReqStr = request_;
    fistReqStr = fistReqStr.substr(0, request_.find_first_of('\n') - 1);
    size_t routeStart = fistReqStr.find_first_of(' ') + 1;
    size_t routeEnd = fistReqStr.find_last_of(' ') - 1;
    if (routeStart != routeEnd) {
        route_ = EraseSpaces(fistReqStr.substr(routeStart, routeEnd));

        size_t queryStart = route_.find('?');
        if (queryStart != std::string::npos) {
            query_ = route_.substr(queryStart);
            route_ = route_.substr(0, queryStart);
        }
        if (route_[route_.length() - 1] != '/')
            route_ = route_ + "/";
        setPath();
    } else {
        route_ = "./";
        path_.push_back("");
    }
    if (DEBUG > 1)
        std::cerr << BgCYAN << "|" << route_ << "|" << RESET << std::endl;
}

void RequestParser::setPath() {
    std::string route = route_.substr(1);
    size_t slash = 0;
    while (slash != std::string::npos) {
        slash = route.find('/');
        if (route.substr(0, slash).length())
            path_.push_back(route.substr(0, slash));
        route = route.substr(slash + 1);
    }
}

void RequestParser::setProtocol() {
    std::string fistReqStr = request_;
    fistReqStr = fistReqStr.substr(0, request_.find_first_of('\n') - 1);
    size_t protocolStart = fistReqStr.find_last_of(' ') + 1;
    size_t protocolEnd = fistReqStr.find_last_of('/') - 1;
    protocol_ = fistReqStr.substr(protocolStart, protocolEnd);
//    std::cerr << "__PROTOCOL___________|" <<  protocol_ << "|" << std::endl;
}

void RequestParser::setHost() {
    host_ = parseByHeaderName("Host:");
//    std::cerr << "__HOST_______________|" << host_ << "|" << std::endl;
}

void RequestParser::setUserAgent() {
    userAgent_ = parseByHeaderName("User-Agent:");
//    std::cerr << "__USER_AGENT_________|" << userAgent_ << "|" << std::endl;
}

void RequestParser::setAccept() {
    accept_ = parseByHeaderName("Accept:");
//    std::cerr << "__ACCEPT_____________|" << accept_ << "|" << std::endl;
}

void RequestParser::setAcceptLanguage() {
    acceptLanguage_ = parseByHeaderName("Accept-Language:");
//    std::cerr << "__ACCEPT_LANGUAGE____|" << acceptLanguage_ << "|" << std::endl;
}

void RequestParser::setAcceptEncoding() {
    acceptEncoding_ = parseByHeaderName("Accept-Encoding:");
//    std::cerr << "__ACCEPT_ENCODING____|" << acceptEncoding_ << "|" << std::endl;
}

void RequestParser::setConnection() {
    connection_ = parseByHeaderName("Connection:");
//    std::cerr << "__CONNECTION_________|" << connection_ << "|" << std::endl;
}

void RequestParser::setSecFetchDest() {
    secFetchDest_ = parseByHeaderName("Sec-Fetch-Dest:");
//    std::cerr << "__SEC_FETCH_DEST_____|" << secFetchDest_ << "|" << std::endl;
}

void RequestParser::setSecFetchMode() {
    secFetchMode_ = parseByHeaderName("Sec-Fetch-Mode:");
//    std::cerr << "__SEC_FETCH_MODE_____|" << secFetchMode_ << "|" << std::endl;
}

void RequestParser::setSecFetchSite() {
    secFetchSite_ = parseByHeaderName("Sec-Fetch-Site:");
//    std::cerr << "__SEC_FETCH_SITE_____|" << secFetchSite_ << "|" << std::endl;
}

void RequestParser::setSecFetchUser() {
    secFetchUser_ = parseByHeaderName("Sec-Fetch-User:");
//    std::cerr << "__SEC_FETCH_USER_____|" << secFetchUser_ << "|" << std::endl;
}

void RequestParser::setCacheControl() {
    cacheControl_ = parseByHeaderName("Cache-Control:");
//    std::cerr << "__CACHE_CONTROL______|" << cacheControl_ << "|" << std::endl;
}

void RequestParser::setBody() {
    std::string erasedRequest = request_;
    size_t bodyStart = request_.find("\r\n\r\n");
    if (bodyStart != std::string::npos) {
        body_ = erasedRequest.substr(bodyStart + 4);
        if (isChunked_)
            body_ = handleChunkedBody();
std::cerr << "__BODY_______________|"<< body_.length() << "|" << std::endl;
}}

void RequestParser::setHeaders() {
    std::string headers = request_.substr(0, request_.find(ENDH));
	std::stringstream ss(headers);
	std::string line;
	while (std::getline(ss, line)) {
		line = line.substr(0, line.find("\r"));
		size_t colonPos = line.find(":");
		if (colonPos != std::string::npos) {
			std::string headerName = line.substr(0, colonPos);
			std::string headerContent = line.substr(colonPos + 2);
            if (headerName == "Transfer-Encoding" && headerContent == "chunked")
                isChunked_ = true;
			headers_.insert(std::pair<std::string, std::string> (headerName, headerContent));
		}
	}
}

void RequestParser::setContentLength() {
    if (!isChunked_) {
        std::string contentLengthStr = parseByHeaderName("Content-Length:");
        contentLength_ = stringToNumber(contentLengthStr);
    } else {
        contentLength_ = body_.length();
    }
//    std::cerr << "__CONTENT_LENGTH_____|" << contentLength_ << "|" << std::endl;
}

void RequestParser::setContentType() {
	if (!headers_.count("Content-Type"))
		headers_["Content-Type"] = "text/html";
	contentType_ = headers_["Content-Type"];
//    std::cerr << "__CONTENT_TYPE_______|" << contentType_ << "|" << std::endl;
}

void RequestParser::setPathInfo(std::string pathInfo) {
    pathInfo_ = pathInfo;
}

void RequestParser::setPathTranslated(std::string pathTranslated) {
    pathTranslated_ = pathTranslated;
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
        setHeaders();
		setContentType();
        if (method_ == "POST" || method_ == "PUT")
            setBody();
        setContentLength();
    } else {
        throw UnsupportedMethodException(method_);
    }
}

std::string RequestParser::handleChunkedBody() {
//    std::string restBody = body_;
    std::string newBody;
    newBody.resize(body_.length());
    size_t lineEnd;
    size_t lineStart = 0;
    while (body_.length()) {
        lineEnd = body_.find("\n", lineStart);
        if (lineEnd != std::string::npos) {
            std::string chunkSize = body_.substr(lineStart, lineEnd - 1);
            int decChunkSize = hexToDec(chunkSize);
            newBody += body_.substr(lineEnd + 1, decChunkSize);
            lineStart = lineEnd + decChunkSize + 2;
//            restBody = restBody.substr(decChunkSize + lineEnd + 1);
        } else
            break;
    }
    return newBody;
}


bool RequestParser::isSupportedMethod() {
    for (size_t i = 0; i < supportedMethods_.size(); i++) {
        if (method_ == supportedMethods_[i])
            return true;
    }
    return false;
}

void RequestParser::showHeaders() {
    std::map<std::string, std::string>::iterator it;
    for (it=headers_.begin(); it!=headers_.end(); it++) {
        std::cerr << GREEN << it->first << RESET <<": " << BLUE << it->second << RESET << std::endl;
    }
}

std::string RequestParser::parseByChar(const std::string &string, char symbol) {
    std::string erasedRequest = request_;
    iterator_ = string.find_first_of(symbol);
    if (iterator_ == std::string::npos)
        return "";
    if (request_[iterator_ - 1] == ' ')
        return erasedRequest.substr(0, iterator_ - 1);
    return erasedRequest.substr(0, iterator_);
}

std::string RequestParser::parseByHeaderName(const std::string &name) {
    std::string erasedRequest = request_;
    iterator_ = request_.find(name);
    if (iterator_ == std::string::npos)
        return "";
    iterator_ += name.size();
    if (request_[iterator_ + 1] == ' ')
        erasedRequest = erasedRequest.erase(0, iterator_);
    else
        erasedRequest = erasedRequest.erase(0, iterator_ + 1);
    return erasedRequest.substr(0, erasedRequest.find_first_of('\n') - 1);
}

std::string RequestParser::EraseSpaces(const std::string &string) {
    size_t spacePos = string.find(' ');
    std::string result = string;
    while (spacePos != std::string::npos) {
        result = result.replace(spacePos, spacePos, "");
        spacePos = result.find(' ');
    }
    return result;
}


RequestParser::UnsupportedMethodException::UnsupportedMethodException(const std::string &method) {
    message_ = RED"Method " + method + " is not supported" RESET;
}

const char *RequestParser::UnsupportedMethodException::what() const throw() {
    return (message_.c_str());
}
