/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Response.hpp                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanfryd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 19:07:21 by ymanfryd          #+#    #+#             */
/*   Updated: 2022/02/20 19:07:27 by ymanfryd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_WEBSERVER_RESPONSE_HPP
#define HTTP_WEBSERVER_RESPONSE_HPP

#include <map>
#include "webserv.hpp"
#include "RequestParser.hpp"

class RequestParser;

class Response {
    private:
            RequestParser               request_;
            std::string                 response_;
            std::string                 requestRoute_;
            std::string                 requestMethod_;
            std::string                 responseHeaders_;
            std::string                 responseContentType_;
            std::string                 responseBody_;
            int                         responseCode_;
            size_t                      contentLength_;
            std::map<int, std::string>  responseCodes_;

            void                        setResponse();
            void                        setResponseCodes();
            void                        setResponseHeaders();
            void                        setContentType();
            void                        setResponseCode(int code);
            void                        setContentLength(size_t len);
            void                        setResponseBody(const std::string& body);

            void                        homeRoot();
            void                        DirectoryRoot();

            void                        createResponse();
            std::string                 getScreen() const;

            Response(): responseCode_(0), contentLength_(0) {};
    public:

            std::string  getResponse()          const;
            std::string  getResponseBody()      const;
            std::string  getResponseHeaders()   const;
            size_t       getContentLength()     const;
            int          getResponseCode()      const;

            explicit    Response(RequestParser &request);
            Response(const Response &other);
            Response &operator=(const Response &other);
            ~Response() {}

            static std::string readContent(const std::string &filename);



};

#endif //HTTP_WEBSERVER_RESPONSE_HPP
