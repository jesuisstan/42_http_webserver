#ifndef HTTP_WEBSERVER_RESPONSE_HPP
#define HTTP_WEBSERVER_RESPONSE_HPP

#include <map>
#include "../config/ServerConfig.hpp"
#include "../../inc/webserv.hpp"
#include "../request/RequestParser.hpp"
#include "../config/Location.hpp"
#include "../cgi/Cgi.hpp"

# define    CGI         0b00000001
# define    CGI_RUN     0b00000010
# define    CGI_ERR		0b00000100

class RequestParser;
class ServerConfig;
class Location;
class Cgi;

class Response {
	private:
			RequestParser                  	RequestParser_;
			ServerConfig                   	ServerConfig_;
			Location                       	Location_;
			std::string                    	response_;
			std::string                    	requestRoute_;
			std::string                    	maxPossibleLocation_;
			std::string                    	requestMethod_;
			std::string                    	requestBody_;
			std::string                    	responseHeaders_;
			std::string                    	responseContentType_;
			std::string                    	responseBody_;
			int                            	responseCode_;
			size_t							ClientMaxBodySize_;
			size_t                         	contentLength_;
			std::vector<std::string>       	requestPath_;
			std::map<int, std::string>     	responseCodes_;
			std::map<std::string, Location>	locations_;
			std::set<std::string>          	locationMethods_;
			std::vector<std::string>       	supportedMethods_;
			std::vector<std::string>       	locationIndex_;
			std::string                    	locationRoot_;
			std::string                    	locationRedirection_;
			std::string                    	requestedFile_;
			std::map<int, std::string>		errorPages_;
			bool                           	cgiRequested_;
			std::set<std::string>           imgExtensions_;
			unsigned char                   cgiFlags_;


			void                           	setResponse();
			void                           	setResponseCodes();
			void                           	setImgExtensions();
			void                           	setResponseHeaders();
			void                           	setContentType();
			void                           	setLocationMethods(const std::set<std::string>& locationMethods);
			void                           	setLocationIndex(const std::set<std::string>& locationIndex);
			void                           	setLocationRoot(const std::string& locationRoot);
			void                           	setLocationRedirection(const std::string& locationRedirection);
			void                           	setResponseCode(int code);
			void                           	setContentLength(size_t len);
			void                           	setResponseBody(const std::string& body);
			void                           	createResponse();
			void                           	checkFileRequested();
			void                           	createAutoIndexPage(const char *dir);
			void                           	readLocationData();
			void                           	trimRequestPath();
			int                            	checkPathForLocation();
			std::string                    	findMaxPossibleLocation(const std::string& location);
			std::string                    	getScreen();
			std::string                    	handleChunkedBody();
			void                           	savePostBody();
			bool                            checkContentLength();

			// cgi block
			void							fillCgiAnswer_();
			void							setCgiCode_();
			void							setCgiBodyLength_();
			void							splitToChunks_();
			bool							chunked_;
			std::vector<std::string>		chunks_;

	public:
			Response(): responseCode_(0), contentLength_(0) {};

			const std::string						&getResponse()          const;
			const std::string						&getResponseBody()      const;
			const std::string						&getResponseHeaders()   const;
			const size_t							&getContentLength()     const;
			const int								&getResponseCode()      const;
			const std::vector<std::string>			&getChunks()			const;
			const bool								&getChunked()			const;
			const unsigned char						&getCgiFlag()			const;

			Response(RequestParser &request, ServerConfig &config);
			Response(const Response &other);
			Response &operator=(const Response &other);
			~Response() {}

			static std::string readContent(const std::string &filename);
			std::string  findFileWithExtension(std::string extension, std::string dir);
};

#endif //HTTP_WEBSERVER_RESPONSE_HPP
