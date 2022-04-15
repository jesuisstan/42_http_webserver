#ifndef HTTP_WEBSERVER__responseHPP
# define HTTP_WEBSERVER__responseHPP

#include <map>
#include "ServerConfig.hpp"
#include "webserv.hpp"
#include "RequestParser.hpp"
#include "Location.hpp"
#include "Cgi.hpp"

# define CGI		0b00000001
# define CGI_RUN	0b00000010
# define CGI_ERR	0b00000100

class RequestParser;
class ServerConfig;
class Location;
class Cgi;

class Response {
private:
	RequestParser					_requestParser;
	ServerConfig					_serverConfig;
	Location						_location;
	std::string						_response;
	std::string						_requestRoute;
	std::string						_maxPossibleLocation;
	std::string						_requestMethod;
	std::string						_requestBody;
	std::string						_responseHeaders;
	std::string						_responseContentType;
	std::string						_responseBody;
	int								_responseCode;
	size_t							_clientMaxBodySize;
	size_t						 	_contentLength;
	std::vector<std::string>		_requestPath;
	std::map<int, std::string>		_responseCodes;
	std::map<std::string, Location>	_locations;
	std::set<std::string>			_locationMethods;
	std::vector<std::string>		_supportedMethods;
	std::vector<std::string>		_locationIndex;
	std::string						_locationRoot;
	std::string						_locationRedirection;
	std::string						_requestedFile;
	std::map<int, std::string>		_errorPages;
	std::set<std::string>			_imgExtensions;
	std::vector<std::string>		_chunks;
	bool							_chunked;
	bool							_cgiRequested;
	unsigned char					_cgiFlags;
	void							setResponse();
	void							setResponseCodes();
	void							setImgExtensions();
	void							setResponseHeaders();
	void							setContentType();
	void							setLocationMethods(const std::set<std::string>& locationMethods);
	void							setLocationIndex(const std::set<std::string>& locationIndex);
	void							setLocationRoot(const std::string& locationRoot);
	void							setLocationRedirection(const std::string& locationRedirection);
	void							setResponseCode(int code);
	void							setContentLength(size_t len);
	void							setResponseBody(const std::string& body);
	void							createResponse();
	void							checkFileRequested();
	void							createAutoIndexPage(const char *dir);
	void							readLocationData();
	void							trimRequestPath();
	int								checkPathForLocation();
	std::string						findMaxPossibleLocation(const std::string& location);
	std::string						getScreen();
	std::string						handleMultipartBody();
	void							savePostBody();
	void							handleDelete();
	bool							checkContentLength();
	void							fillCgiAnswer_();
	void							setCgiCode_();
	void							setCgiBodyLength_();
	void							splitTo_chunks();

public:
	Response(): _responseCode(0), _contentLength(0) {};
	Response(RequestParser &request, ServerConfig &config);
	Response(const Response &other);
	~Response() {}
	Response 						&operator=(const Response &other);
	const std::string				&getResponse()		  const;
	const std::string				&getResponseBody()	  const;
	const std::string				&getResponseHeaders()   const;
	const size_t					&getContentLength()	 const;
	const int						&getResponseCode()	  const;
	const std::vector<std::string>	&getChunks()			const;
	const bool						&getChunked()			const;
	const unsigned char				&getCgiFlag()			const;
	static std::string				readContent(const std::string &filename);
	std::string						findFileWithExtension(std::string extension, std::string dir);
};

#endif //HTTP_WEBSERVER__responseHPP
