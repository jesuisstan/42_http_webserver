#ifndef HTTP_WEBSERVER__requestPARSER_HPP
# define HTTP_WEBSERVER__requestPARSER_HPP

#include "webserv.hpp"
#include "Logger.hpp"

class RequestParser {
private:
	std::string							_host;
	std::string							_body;
	std::string							_route;
	std::string							_query;
	std::string							_accept;
	std::string							_method;
	std::string							_request;
	std::string							_pathInfo;
	std::string							_protocol;
	std::string							_userAgent;
	std::string							_connection;
	std::string							_contentType;
	std::string							_secFetchDest;
	std::string							_secFetchMode;
	std::string							_secFetchSite;
	std::string							_secFetchUser;
	std::string							_cacheControl;
	std::string							_acceptLanguage;
	std::string							_acceptEncoding;
	std::string							_pathTranslated;
	bool								_isChunked;
	bool								_isMiltipart;
	std::string							_boundary;
	std::vector<std::string>			_path;
	size_t								_iterator;
	size_t								_contentLength;
	std::map<std::string, std::string>	_headers;
	std::vector<std::string>			_supportedMethods;
	void								setMethod();
	void								setRoute();
	void								setPath();
	void								setProtocol();
	void								setHost();
	void								setUserAgent();
	void								setAccept();
	void								setAcceptLanguage();
	void								setAcceptEncoding();
	void								setConnection();
	void								setSecFetchDest();
	void								setSecFetchMode();
	void								setSecFetchSite();
	void								setSecFetchUser();
	void								setCacheControl();
	void								setBody();
	void								setHeaders();
	void								setContentLength();
	void								setContentType();
	void								parse();
	void								setSupportedMethods();
	std::string							parseByChar(const std::string& string, char symbol);
	std::string							parseByHeaderName(const std::string& name);
	std::string							handleChunkedBody();
	static std::string					EraseSpaces(const std::string& string);

public:
	RequestParser();
	RequestParser(std::string request, size_t requestLen);
	RequestParser(const RequestParser &other);
	RequestParser &operator=(const RequestParser &other);
	~RequestParser() {}
	void								setPathInfo(std::string pathInfo);
	void								setPathTranslated(std::string pathTranslated);
	void								showHeaders();
	bool								isSupportedMethod();
	const   bool						&getMultipart() const;
	const   size_t						&getContentLength() const;
	const   std::string					&getRequest() const;
	const   std::string					&getMethod() const;
	const   std::string					&getRoute() const;
	const   std::string					&getQuery() const;
	const   std::string					&getProtocol() const;
	const   std::string					&getHost() const;
	const   std::string					&getUserAgent() const;
	const   std::string					&getAccept() const;
	const   std::string					&getAcceptLanguage() const;
	const   std::string					&getAcceptEncoding() const;
	const   std::string					&getConnection() const;
	const   std::string					&getSecFetchDest() const;
	const   std::string					&getSecFetchMode() const;
	const   std::string					&getSecFetchSite() const;
	const   std::string					&getSecFetchUser() const;
	const   std::string					&getCacheControl() const;
	const   std::string					&getBody() const;
	const   std::string					&getPathInfo() const;
	const   std::string					&getPathTranslated() const;
	const   std::string					&getContentType() const;
	const   std::string					&getBoundary() const;
	const   std::vector<std::string>	&getPath() const;
	const   std::vector<std::string>	&getSupportedMethods() const;
	const   std::map<std::string, std::string>  &getHeaders() const;
	
	class UnsupportedMethodException: public std::exception {
		private:
			std::string		 message_;
		public:
			explicit UnsupportedMethodException(std::string const& method);
			const char* what() const throw();
			virtual ~UnsupportedMethodException() throw() {}
		};
};

#endif //HTTP_WEBSERVER__requestPARSER_HPP
