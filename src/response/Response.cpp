#include "Response.hpp"

Response::Response(RequestParser &request, ServerConfig &config) :
		_requestParser(request),
		_serverConfig(config),
		_requestRoute(request.getRoute()),
		_requestMethod(request.getMethod()),
		_requestBody(request.getBody()),
		_responseCode(0),
		_clientMaxBodySize(config.getClientMaxBodySize()),
		_contentLength(0),
		_requestPath(request.getPath()),
		_locations(config.getLocations()),
		_supportedMethods(request.getSupportedMethods()),
		_errorPages(config.getErrorPages()),
		_cgiRequested(false) {
	_chunked = false;
	_cgiFlags = 0;
	setResponseCodes();
	setImgExtensions();
	createResponse();
}

Response::Response(const Response &other) {
	*this = other;
}

Response &Response::operator=(const Response &other) {
	if (this != &other) {
		_requestParser		  = other._requestParser;
		_serverConfig		   = other._serverConfig;
		_location			   = other._location;
		_requestRoute		   = other._requestRoute;
		_requestPath			= other._requestPath;
		_requestMethod		  = other._requestMethod;
		_requestedFile		  = other._requestedFile;
		_response			   = other._response;
		_responseCode		   = other._responseCode;
		_responseBody		   = other._responseBody;
		_responseHeaders		= other._responseHeaders;
		_responseContentType	= other._responseContentType;
		_responseCodes		  = other._responseCodes;
		_contentLength		  = other._contentLength;
		_supportedMethods	   = other._supportedMethods;
		_locations			  = other._locations;
		_locationMethods		= other._locationMethods;
		_locationIndex		  = other._locationIndex;
		_locationRoot		   = other._locationRoot;
		_locationRedirection	= other._locationRedirection;
		_errorPages			 = other._errorPages;
		_clientMaxBodySize	  = other._clientMaxBodySize;
		_requestBody			= other._requestBody;
		_cgiRequested		   = other._cgiRequested;
		_maxPossibleLocation	= other._maxPossibleLocation;
		_chunked				= other._chunked;
		_chunks					= other._chunks;
	}
	return *this;
}

/**************************/
/******** GETTERS *********/
/**************************/

const std::string &Response::getResponse() const {
	return this->_response;
}

const int &Response::getResponseCode() const {
	return this->_responseCode;
}

const size_t &Response::getContentLength() const {
	return this->_contentLength;
}

const std::string &Response::getResponseBody() const {
	return this->_responseBody;
}

const std::string &Response::getResponseHeaders() const {
	return this->_responseHeaders;
}

const std::vector<std::string> &Response::getChunks() const {
	return this->_chunks;
}

const bool &Response::getChunked() const {
	return this->_chunked;
}

const unsigned char	&Response::getCgiFlag()	const {
	return _cgiFlags;
}


/**************************/
/******** SETTERS *********/
/**************************/

void Response::setResponse() {
	_response = _responseHeaders + ENDH + _responseBody;
}

void Response::setResponseCode(int code) {
	_responseCode = code;
}

void Response::setContentType() {
	if (!_requestPath.size()) {
		_responseContentType = "text/html\n";
		return ;
	}
	size_t routeEnd = _requestPath.size() - 1;
	size_t dotPos = _requestPath[routeEnd].find('.');
	if (dotPos != std::string::npos) {
		std::string extension = _requestPath[routeEnd].substr(dotPos);
		if (extension == ".css") {
			_requestedFile = _requestPath[routeEnd];
			_responseContentType = "text/css\n";
		} else if (extension == ".js") {
			_requestedFile = _requestPath[routeEnd];
			_responseContentType = "application/javascript\n";
		}  else if (extension == ".html") {
			_requestedFile = _requestPath[routeEnd];
			_responseContentType = "text/html\n";
		} else if (_imgExtensions.count(extension)) {
			_requestedFile = _requestPath[routeEnd];
			_responseContentType = "image\n";
		} else if (extension == _serverConfig.getCgiExt())
			_requestedFile = _requestPath[routeEnd];
		 else
			_responseContentType = "application/octet-stream\n";
	} else
		_responseContentType = "text/html\n";
}

void Response::setResponseHeaders() {
	_responseHeaders = "HTTP/1.1 ";
	_responseHeaders += _responseCodes.find(_responseCode)->second;
	if (_responseCode == 301)
		_responseHeaders += "Location: " + _locationRedirection + _requestRoute + "\n";
	_responseHeaders += "Content-Type: " + _responseContentType;
	_responseHeaders += "Content-Length: ";
	_responseHeaders += numberToString(_contentLength);
}

void Response::setResponseBody(const std::string &body) {
	_responseBody = body;
}

void Response::setContentLength(size_t len) {
	_contentLength = len;
}

void Response::setLocationMethods(const std::set<std::string> &locationMethods) {
	for (size_t j = 0; j < _supportedMethods.size(); j++) {
		if (locationMethods.count(_supportedMethods[j])) {
			_locationMethods.insert(_supportedMethods[j]);
		}
	}
}

void Response::setLocationIndex(const std::set<std::string> &locationIndex) {
	std::set<std::string>::iterator it;
	for (it = locationIndex.begin(); it != locationIndex.end(); it++) {
		_locationIndex.push_back(it->data());
	}
}

void Response::setLocationRoot(const std::string &locationRoot) {
	_locationRoot = locationRoot;
}

void Response::setLocationRedirection(const std::string& locationRedirection) {
	_locationRedirection = locationRedirection;
}

void Response::setResponseCodes() {
	_responseCodes.insert(std::pair<int, std::string>(200, "200 OK\n"));
	_responseCodes.insert(std::pair<int, std::string>(201, "201 Created\n"));
	_responseCodes.insert(std::pair<int, std::string>(301, "301 Moved Permanently\n"));
	_responseCodes.insert(std::pair<int, std::string>(400, "400 Bad Request\n"));
	_responseCodes.insert(std::pair<int, std::string>(404, "404 Not Found\n"));
	_responseCodes.insert(std::pair<int, std::string>(405, "405 Method Not Allowed\n"));
	_responseCodes.insert(std::pair<int, std::string>(413, "413 Request Entity Too Large\n"));
	_responseCodes.insert(std::pair<int, std::string>(500, "500 Internal Server Error\n"));
	_responseCodes.insert(std::pair<int, std::string>(502, "502 Bad Gateway\n"));
	_responseCodes.insert(std::pair<int, std::string>(503, "503 Service Unavailable\n"));
}

void Response::setImgExtensions() {
	_imgExtensions.insert(".png");
	_imgExtensions.insert(".jpeg");
	_imgExtensions.insert(".jpg");
	_imgExtensions.insert(".gif");
}

/**************************/
/******** HELPERS *********/
/**************************/


void Response::createResponse() {
	std::string body;
	readLocationData();
	setContentType();
	int isRightPath = checkPathForLocation();
	if (!_locationRedirection.empty())
		setResponseCode(301);
	else if (!_locationMethods.count(_requestMethod) && !_cgiRequested)
		setResponseCode(405);
	else if (isRightPath == -1)
		setResponseCode(404);
	else if (_cgiFlags & CGI)
		return ;
	else if (_requestBody.length() > _clientMaxBodySize)
		setResponseCode(413);
	else if (!checkContentLength())
		setResponseCode(400);
	else if (!_cgiRequested && _requestMethod != "PUT" && _requestMethod != "POST")
		setResponseCode(200);
	else
		setResponseCode(201);
	if (_responseCode == 201)
		savePostBody();
	if (_requestMethod == "DELETE")
		handleDelete();
	if (_responseBody.empty() || _responseCode > 300) {
		body = readContent(getScreen());
		if (_responseBody.empty() || _responseCode > 300)  {
			setContentLength(body.size());
			setResponseHeaders();
			setResponseBody(body);
		} else {
			setContentLength(_responseBody.size());
			setResponseHeaders();
		}
	} else {
		setContentLength(_responseBody.size());
		setResponseHeaders();
	}
	setResponse();
}

bool Response::checkContentLength() {
	size_t contentLength = _requestParser.getContentLength();
	if (contentLength) {
		size_t bodyLength = _requestParser.getBody().length();
		if (contentLength != bodyLength)
			return false;
	}
	return true;
}

void Response::checkFileRequested() {
	size_t routeEnd = _requestPath.size() - 1;
	size_t dotPos = _requestPath[routeEnd].find('.');
	if (dotPos != std::string::npos) {
		std::string extension = _requestPath[routeEnd].substr(dotPos);
		if (extension == ".css") {
			_requestedFile = _requestPath[routeEnd];
			_responseContentType = "text/css\n";
		} else if (extension == ".js") {
			_requestedFile = _requestPath[routeEnd];
			_responseContentType = "application/javascript\n";
		}
	}
}

std::string Response::handleMultipartBody() {
	size_t fileNameStart = _requestBody.find("filename=");
	if (fileNameStart != std::string::npos)
		fileNameStart += 10;
	size_t fileNameEnd = _requestBody.find('\"', fileNameStart);
	if (fileNameEnd != std::string::npos)
		fileNameEnd += 1;
	size_t bodyStart = _requestBody.find(ENDH);
	if (bodyStart != std::string::npos)
		bodyStart += 4;
	std::string finalBoundary = _requestParser.getBoundary() + "--";
	size_t bodyEnd = _requestBody.find(finalBoundary);
	if (bodyEnd != std::string::npos)
		bodyEnd -= 4;
	if (fileNameStart != std::string::npos && fileNameEnd != std::string::npos)
		_requestedFile = _requestBody.substr(fileNameStart, fileNameEnd - fileNameStart);
	if (bodyStart != std::string::npos && bodyEnd != std::string::npos)
		_requestBody = _requestBody.substr(bodyStart, bodyEnd - bodyStart);
	return _requestBody;
}

void Response::savePostBody() {
	int filesCount = 0;
	struct dirent *d;
	DIR *dh = opendir(_locationRoot.c_str());
	if (dh == NULL)
		return ;
	while ((d = readdir(dh)) != NULL)
	{ filesCount++; }
	std::string filesCountStr = numberToString(filesCount - 1);
	if (_requestParser.getMultipart())
		_requestBody = handleMultipartBody();
	std::string filename = _requestedFile.empty() ? filesCountStr : _requestedFile.substr(0, _requestedFile.length() - 1);
	std::ofstream postBodyFile((_locationRoot + "/" + filename).c_str());
	postBodyFile << _requestBody;
	postBodyFile.close();
	closedir(dh);
}

void Response::handleDelete() {
	struct stat s;
	std::string strPath = _locationRoot + _requestRoute.substr(0, _requestRoute.length() -1);
	const char * path = strPath.c_str();
	if( stat(path,&s) == 0 ) {
		if( s.st_mode & S_IFDIR ) {
			return;
		} else {
			std::remove(path);
		}
	}
}

std::string Response::findMaxPossibleLocation(const std::string &location) {
	std::string route = _requestRoute;

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

	for (iterator = _locations.begin(); iterator != _locations.end(); iterator++) {
		if (!findMaxPossibleLocation(iterator->first).empty())
			maxPossibleLocation = findMaxPossibleLocation(iterator->first);
	}
	if (maxPossibleLocation.empty())
		maxPossibleLocation = "/";
	for (iterator = _locations.begin(); iterator != _locations.end(); iterator++) {
		if (iterator->first == maxPossibleLocation) {
			_location = iterator->second;
			setLocationRedirection(iterator->second.getRedirection());
			setLocationRoot(iterator->second.getAlias());
			setLocationMethods(iterator->second.getMethods());
			setLocationIndex(iterator->second.getIndex());
			int locationBodySize = iterator->second.getClientMaxBodySize();
			if (locationBodySize > 0)
				_clientMaxBodySize = locationBodySize;
		}
	}
	if (maxPossibleLocation != "/")
		_requestRoute = _requestRoute.substr(maxPossibleLocation.length() - 1);
	_maxPossibleLocation = maxPossibleLocation.substr(0, maxPossibleLocation.length() - 1);
}


int Response::checkPathForLocation() {
	std::string stringFilename;
	if (_locationRoot[_locationRoot.size() - 1] == '/')
		stringFilename = _locationRoot + _requestRoute.substr(1, _requestRoute.length() - 2);
	else
		stringFilename = _locationRoot + "/" + _requestRoute.substr(1, _requestRoute.length() - 2);
	char *filename = const_cast<char *>(stringFilename.c_str());
	int openRes = open(filename, O_RDONLY);
	if (openRes == -1 && _requestMethod != "PUT" && _requestMethod != "POST")
		return -1;
	close(openRes);
	if (_requestedFile.find('.') != std::string::npos) {
		std::string extension = _requestedFile.substr(_requestedFile.find('.'));
		if (extension[extension.length() - 1] == '/')
			extension = extension.substr(0, extension.length() -1);
		if (extension == _serverConfig.getCgiExt()) {
			_cgiRequested = true;
			std::string pathInfo = stringFilename.substr(1);
			_requestParser.setPathInfo(pathInfo);
			char cwd[1024];
			getcwd(cwd, sizeof(cwd));
			std::string path = (std::string) cwd;
			_requestParser.setPathTranslated(cwd + stringFilename.substr(1));
			if (DEBUG > 1) {
				std::stringstream str;
				str << BgRED << "CGI START";
				Logger::printDebugMessage(&str);
			}
			Cgi cgi(_serverConfig, _requestParser);
			_cgiFlags |= CGI;
			std::pair<int, std::string> cgiResult = cgi.execute();
			_responseCode = cgiResult.first;
			_response = cgiResult.second;
			fillCgiAnswer_();
			return 1;
		}
	} else if (_requestMethod == "PUT" || _requestMethod == "POST") {
		_requestedFile = _requestRoute.substr(1);
		return 1;
	}
	std::string content = readContent(filename);
	if (!content.empty()) {
		setResponseBody(content);
		return 1;
	} else {
		if (_location.getAutoindex()) {
			createAutoIndexPage(stringFilename.c_str());
			return 1;
		}
		std::string index;
		if (_locationIndex[0][0] != '.') {
			std::string file = stringFilename + _locationIndex[0];
			int fd = open(file.c_str(), O_RDONLY);
			if (fd == -1 && _requestMethod != "PUT") {
				if (_requestMethod == "PUT")
				   close(fd);
				return -1;
			}
			close(fd);
			index = readContent(stringFilename + "/" + _locationIndex[0]);
		}
		else
			index = findFileWithExtension(_locationIndex[0], stringFilename);
		setResponseBody(index);
		return 1;
	}
}

void Response::fillCgiAnswer_() {
	std::string ans;
	std::stringstream str;

	if (DEBUG > 0) {
		ans = _response.substr(0, std::min(200, (int)_response.size()));
		str << BLUE << "Cgi response. First 200 from " 
					<< ans.size() << " chars\n" << RESET << ans;
		Logger::printInfoMessage(&str);
	}
	setCgiCode_();
	setCgiBodyLength_();
	if (DEBUG > 0) {
		if (_chunked)
			ans = _chunks[0].substr(0, 200);
		else
			ans = _response.substr(0, 200);
		str << BLUE << "Cgi after procceccing. First 200 from " 
					<< ans.size() << " chars\n" << RESET << ans;;
		Logger::printInfoMessage(&str);
	}
}

void Response::setCgiCode_() {
	size_t	statusPos = _response.find("Status: ");
	size_t	statusNumberPos = _response.substr(statusPos, 15).find("200", statusPos);
	if (_responseCode == 200 and statusPos != std::string::npos and statusNumberPos != std::string::npos)
		_response = "HTTP/1.1 200 OK\r\n" + _response;
	else
		_response = "HTTP/1.1 500 KO\r\n" + _response;
}


void Response::setCgiBodyLength_() {
	size_t bodySize  = 0;
	size_t	headersEndPos = _response.find(ENDH);
	if (headersEndPos != std::string::npos)
		bodySize = _response.size() - headersEndPos - 4;
	setContentLength(bodySize);
	if (bodySize < NEED_CHUNKS) {
		std::string contentLen = "\r\nContent-Length: " + numberToString(bodySize);
		_response.insert(std::min(headersEndPos, _response.size()), contentLen);
	}
	else {
		std::string contentLen = "\r\nTransfer-Encoding: chunked";
		_response.insert(headersEndPos, contentLen);

		splitTo_chunks();
	}
}

void Response::splitTo_chunks() {
	size_t		pos;
	size_t		headersEndPos;
	size_t		leftSizeChunk;
	std::string	hexString;
	std::string first_chunk;
	std::stringstream str;
	
	_chunked = true;
	headersEndPos = _response.find(ENDH) + 4;

	
	leftSizeChunk = CHUNK_SIZE - headersEndPos;
	hexString = getHex(leftSizeChunk) + CRLF;
	first_chunk = _response.substr(0, headersEndPos) + hexString + _response.substr(headersEndPos, leftSizeChunk) + CRLF;
	_chunks.push_back(first_chunk);
	if (DEBUG > 1) {
		str << "chunks size: " << first_chunk.size() << ", first 50:\n" << first_chunk.substr(0, 50);
		Logger::printDebugMessage(&str);
	}
	pos = CHUNK_SIZE;
	while (1) {
		std::string chunk;
		leftSizeChunk = std::min(_response.size() - pos, (size_t)CHUNK_SIZE);
		hexString = getHex(leftSizeChunk) + CRLF;
		chunk = hexString + _response.substr(pos, leftSizeChunk) + CRLF;
		if (DEBUG > 1) {
			str << "chunks size: " << chunk.size() << ", first 50:\n" << chunk.substr(0, 50);
			Logger::printDebugMessage(&str);
		}
		_chunks.push_back(chunk);
		pos += leftSizeChunk;
		if (!leftSizeChunk)
			break;
	}
	_response = "";
	_responseBody = "";
}

std::string Response::findFileWithExtension(std::string extension, std::string dir) {
	struct dirent *d;
	DIR *dh = opendir(dir.c_str());
	if (dh == NULL)
		return "";
	while ((d = readdir(dh)) != NULL)
	{
		std::string file = (std::string) d->d_name;
		size_t dotPos = file.find('.');
		if (dotPos != std::string::npos) {
			std::string fileExtension = file.substr(dotPos);
			if (fileExtension == extension) {
				closedir(dh);
				return readContent(dir + "/" + file);
			}

		}
	}
	closedir(dh);
	return "";
}

void	Response::createAutoIndexPage(const char *dir) {
	struct dirent *d;
	DIR *dh = opendir(dir);
	if (dh == NULL)
		return ;
	std::string strDir = (std::string)dir;
	std::string autoIndexPage = readContent("./www/screens/sample.html");
	autoIndexPage += "<body>\n <h1 class=\"autoIndexHeader\">";
	autoIndexPage += dir;
	autoIndexPage += "</h1>\n<div class=\"simpleContainer\">";
	strDir = strDir[strDir.size() - 1] == '/' ? strDir.substr(0, strDir.size() - 1) : strDir;
	while ((d = readdir(dh)) != NULL)
	{
		autoIndexPage += "<a class=\"autoIndexLink\"href=" + _maxPossibleLocation + _requestRoute + (std::string)d->d_name + ">";
		autoIndexPage +=  (std::string)d->d_name + "</a>\n";
	}
	autoIndexPage += "<div>\n</body>\n</html>\n";
	setResponseBody(autoIndexPage);
	closedir(dh);
}

std::string Response::readContent(const std::string &filename) {
	std::string buf;
	std::string content;
	char buffer[SIZE_BUF_TO_RCV];
	int received = 1;
	int file = open(filename.c_str(), O_RDONLY);
	while (received) {
		received = read(file, buffer, SIZE_BUF_TO_RCV);
		if (received < 0) {
			close(file);
			return "";
		}
		else if (received) {
			buf = std::string(buffer, received);
			content += buf;
		}
	}
	close(file);
	return content;
}

std::string Response::getScreen() {
	std::string filename = _locationRoot;
	if (_errorPages.find(_responseCode) != _errorPages.end())
		filename = _errorPages[_responseCode];
	else {
		std::string errorDescription = _responseCodes.find(_responseCode)->second;
		setResponseBody(errorDescription);
		filename =  errorDescription;
	}
	return filename;
}

