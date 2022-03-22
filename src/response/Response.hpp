/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanfryd <ymanfryd@student.21-school.ru    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 19:07:21 by ymanfryd          #+#    #+#             */
/*   Updated: 2022/03/22 11:52:05 by ymanfryd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_WEBSERVER_RESPONSE_HPP
#define HTTP_WEBSERVER_RESPONSE_HPP

#include <map>
#include "../config/ServerConfig.hpp"
#include "../../inc/webserv.hpp"
#include "../request/RequestParser.hpp"
#include "../config/Location.hpp"
#include "../cgi/Cgi.hpp"

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
            void                           	setResponse();
            void                           	setResponseCodes();
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

            Response(): responseCode_(0), contentLength_(0) {};
    public:

            const std::string  &getResponse()          const;
            const std::string  &getResponseBody()      const;
            const std::string  &getResponseHeaders()   const;
            const size_t       &getContentLength()     const;
            const int          &getResponseCode()      const;

            Response(RequestParser &request, ServerConfig &config);
            Response(const Response &other);
            Response &operator=(const Response &other);
            ~Response() {}

            static std::string readContent(const std::string &filename);
            std::string  findFileWithExtension(std::string extension, std::string dir);
};

#endif //HTTP_WEBSERVER_RESPONSE_HPP
