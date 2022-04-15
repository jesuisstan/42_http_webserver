#include "RequestParser.hpp"

RequestParser::RequestParser() {
}

RequestParser::RequestParser(std::string request, size_t requestLen) {
	_request = request.substr(0, requestLen);
	parse();
}

RequestParser::RequestParser(const RequestParser &other) {
	*this = other;
}

RequestParser &RequestParser::operator=(const RequestParser &other) {
	if (this != &other) {
		_host			 = other._host;
		_body			 = other._body;
		_route			= other._route;
		_query			= other._query;
		_accept		   = other._accept;
		_method		   = other._method;
		_headers		  = other._headers;
		_request		  = other._request;
		_pathInfo		 = other._pathInfo;
		_protocol		 = other._protocol;
		_userAgent		= other._userAgent;
		_connection	   = other._connection;
		_contentType	  = other._contentType;
		_secFetchDest	 = other._secFetchDest;
		_secFetchMode	 = other._secFetchMode;
		_secFetchSite	 = other._secFetchSite;
		_secFetchUser	 = other._secFetchUser;
		_cacheControl	 = other._cacheControl;
		_acceptLanguage   = other._acceptLanguage;
		_acceptEncoding   = other._acceptEncoding;
		_pathTranslated   = other._pathTranslated;
		_isChunked		= other._isChunked;
		_isMiltipart	  = other._isMiltipart;
		_boundary		 = other._boundary;

		_path			 = other._path;
		_iterator		 = other._iterator;
		_contentLength	= other._contentLength;
		_supportedMethods = other._supportedMethods;
	}
	return *this;
}

/**************************/
/******** GETTERS *********/
/**************************/

const std::string &RequestParser::getRequest() const {
	return this->_request;
}

const std::string &RequestParser::getMethod() const {
	return this->_method;
}

const std::string &RequestParser::getRoute() const {
	return this->_route;
}

const std::string &RequestParser::getQuery() const {
	return this->_query;
}

const std::string &RequestParser::getProtocol() const {
	return this->_protocol;
}


const std::string &RequestParser::getHost() const {
	return this->_host;
}

const std::string &RequestParser::getUserAgent() const {
	return this->_userAgent;
}

const std::string &RequestParser::getAccept() const {
	return this->_accept;
}

const std::string &RequestParser::getAcceptLanguage() const {
	return this->_acceptLanguage;
}

const std::string &RequestParser::getAcceptEncoding() const {
	return this->_acceptEncoding;
}

const std::string &RequestParser::getConnection() const {
	return this->_connection;
}

const std::string &RequestParser::getSecFetchDest() const {
	return this->_secFetchDest;
}

const std::string &RequestParser::getSecFetchMode() const {
	return this->_secFetchMode;
}

const std::string &RequestParser::getSecFetchSite() const {
	return this->_secFetchSite;
}

const std::string &RequestParser::getSecFetchUser() const {
	return this->_secFetchUser;
}

const std::string &RequestParser::getCacheControl() const {
	return this->_cacheControl;
}

const std::string &RequestParser::getBody() const {
	return this->_body;
}

const std::map<std::string, std::string> &RequestParser::getHeaders() const {
	return this->_headers;
}

const size_t &RequestParser::getContentLength() const {
	return this->_contentLength;
}

const std::string &RequestParser::getContentType() const {
	return this->_contentType;
}

const std::string &RequestParser::getPathInfo() const {
	return this->_pathInfo;
}

const std::vector <std::string> &RequestParser::getPath() const {
	return this->_path;
}

const std::vector <std::string> &RequestParser::getSupportedMethods() const {
	return this->_supportedMethods;
}

const std::string &RequestParser::getPathTranslated() const {
	return this->_pathTranslated;
}

const bool &RequestParser::getMultipart() const {
	return this->_isMiltipart;
}

const std::string &RequestParser::getBoundary() const {
	return this->_boundary;
}

/**************************/
/******** SETTERS *********/
/**************************/


void RequestParser::setMethod() {
	_method = parseByChar(_request, ' ');
}

void RequestParser::setRoute() {
	std::string fistReqStr = _request.substr(0, _request.find('\n') - 1);
	size_t routeStart = fistReqStr.find(' ') + 1;
	size_t routeEnd = fistReqStr.rfind(' ') - 1;
	if (routeStart != routeEnd) {
		_route = fistReqStr.substr(routeStart, routeEnd - routeStart + 1);
		size_t queryStart = _route.find('?');
		if (queryStart != std::string::npos) {
			_query = _route.substr(queryStart);
			_route = _route.substr(0, queryStart);
		}
		if (_route[_route.length() - 1] != '/')
			_route = _route + "/";
		setPath();
	} else {
		_route = "./";
		_path.push_back("");
	}
	std::stringstream str;
	Logger::printDebugMessage(&str);
}

void RequestParser::setPath() {
	std::string route = _route.substr(1);
	size_t slash = 0;
	while (slash != std::string::npos) {
		slash = route.find('/');
		if (route.substr(0, slash).length())
			_path.push_back(route.substr(0, slash));
		route = route.substr(slash + 1);
	}
}

void RequestParser::setProtocol() {
	std::string fistReqStr = _request;
	fistReqStr = fistReqStr.substr(0, _request.find_first_of('\n') - 1);
	size_t protocolStart = fistReqStr.find_last_of(' ') + 1;
	size_t protocolEnd = fistReqStr.find_last_of('/') - 1;
	_protocol = fistReqStr.substr(protocolStart, protocolEnd);
}

void RequestParser::setHost() {
	_host = parseByHeaderName("Host:");
}

void RequestParser::setUserAgent() {
	_userAgent = parseByHeaderName("User-Agent:");
}

void RequestParser::setAccept() {
	_accept = parseByHeaderName("Accept:");
}

void RequestParser::setAcceptLanguage() {
	_acceptLanguage = parseByHeaderName("Accept-Language:");
}

void RequestParser::setAcceptEncoding() {
	_acceptEncoding = parseByHeaderName("Accept-Encoding:");
}

void RequestParser::setConnection() {
	_connection = parseByHeaderName("Connection:");
}

void RequestParser::setSecFetchDest() {
	_secFetchDest = parseByHeaderName("Sec-Fetch-Dest:");
}

void RequestParser::setSecFetchMode() {
	_secFetchMode = parseByHeaderName("Sec-Fetch-Mode:");
}

void RequestParser::setSecFetchSite() {
	_secFetchSite = parseByHeaderName("Sec-Fetch-Site:");
}

void RequestParser::setSecFetchUser() {
	_secFetchUser = parseByHeaderName("Sec-Fetch-User:");
}

void RequestParser::setCacheControl() {
	_cacheControl = parseByHeaderName("Cache-Control:");
}

void RequestParser::setBody() {
	std::string erasedRequest = _request;
	size_t bodyStart = _request.find(ENDH);
	if (bodyStart != std::string::npos) {
		_body = erasedRequest.substr(bodyStart + 4);
		if (_isChunked)
			_body = handleChunkedBody();
}}

void RequestParser::setHeaders() {
	std::string headers = _request.substr(0, _request.find(ENDH));
	std::stringstream ss(headers);
	std::string line;
	while (std::getline(ss, line)) {
		size_t colonPos = line.find(":");
		if (colonPos != std::string::npos) {
			std::string headerName = line.substr(0, colonPos);
			std::string headerContent = line.substr(colonPos + 1);
			headerContent = cutStringSpacesAndCr(headerContent);
			if (headerName == "Transfer-Encoding" && headerContent == "chunked")
				_isChunked = true;
			if (headerName == "Content-Type" && headerContent.find("multipart/form-data;") != std::string::npos) {
				_isMiltipart = true;
				size_t boundaryStart = headerContent.find("boundary=") + 9;
				size_t boundaryEnd = headerContent.length();
				_boundary =  headerContent.substr(boundaryStart, boundaryEnd - boundaryStart);
			}

			_headers.insert(std::pair<std::string, std::string> (headerName, headerContent));
		}
	}
}

void RequestParser::setContentLength() {
	if (!_isChunked) {
		std::string contentLengthStr = parseByHeaderName("Content-Length:");
		_contentLength = stringToNumber(contentLengthStr);
	} else {
		_contentLength = _body.length();
	}
}

void RequestParser::setContentType() {
	if (!_headers.count("Content-Type"))
		_headers["Content-Type"] = "text/html";
	_contentType = _headers["Content-Type"];
}

void RequestParser::setPathInfo(std::string pathInfo) {
	_pathInfo = pathInfo;
}

void RequestParser::setPathTranslated(std::string pathTranslated) {
	_pathTranslated = pathTranslated;
}

void RequestParser::setSupportedMethods() {
	_supportedMethods.push_back("GET");
	_supportedMethods.push_back("POST");
	_supportedMethods.push_back("DELETE");
	_supportedMethods.push_back("HEAD");
	_supportedMethods.push_back("PUT");
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
		if (_method == "POST" || _method == "PUT")
			setBody();
		setContentLength();
	} else {
		 throw UnsupportedMethodException(_method);
	 }
}

std::string RequestParser::handleChunkedBody() {
	std::string newBody;
	newBody.reserve(_body.length());
	size_t lineEnd;
	size_t lineStart = 0;
	while (_body.length()) {
		lineEnd = _body.find("\n", lineStart);
		if (lineEnd != std::string::npos) {
			std::string chunkSize = _body.substr(lineStart, lineEnd - lineStart - 1);
			int decChunkSize = hexToDec(chunkSize);
			newBody += _body.substr(lineEnd + 1, decChunkSize);
			lineStart = lineEnd + decChunkSize + 3;
		} else
			break;
	}
	return newBody;
}


bool RequestParser::isSupportedMethod() {
	for (size_t i = 0; i < _supportedMethods.size(); i++) {
		if (_method == _supportedMethods[i])
			return true;
	}
	return false;
}

void RequestParser::showHeaders() {
	std::map<std::string, std::string>::iterator it;
	std::stringstream str;
	for (it=_headers.begin(); it!=_headers.end(); it++) {
		str << GREEN << it->first << RESET <<": " << BLUE << it->second << RESET << std::endl;
		Logger::printDebugMessage(&str);
	}
}

std::string RequestParser::parseByChar(const std::string &string, char symbol) {
	_iterator = string.find(symbol);
	if (_iterator == std::string::npos)
		return "";
	if (_request[_iterator - 1] == ' ')
		return _request.substr(0, _iterator - 1);
	return _request.substr(0, _iterator);
}

std::string RequestParser::parseByHeaderName(const std::string &name) {
	std::string erasedRequest = _request;
	_iterator = _request.find(name);
	if (_iterator == std::string::npos)
		return "";
	_iterator += name.size();
	if (_request[_iterator + 1] == ' ')
		erasedRequest = erasedRequest.erase(0, _iterator);
	else
		erasedRequest = erasedRequest.erase(0, _iterator + 1);
	return erasedRequest.substr(0, erasedRequest.find_first_of('\n') - 1);
}

RequestParser::UnsupportedMethodException::UnsupportedMethodException(const std::string &method) {
	message_ = RED"Method " + method + " is not supported" + RESET;
}

const char *RequestParser::UnsupportedMethodException::what() const throw() {
	return (message_.c_str());
}
