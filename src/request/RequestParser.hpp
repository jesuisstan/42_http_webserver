/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   RequestParser.hpp                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ymanfryd <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/20 19:07:06 by ymanfryd          #+#    #+#             */
/*   Updated: 2022/02/20 19:07:14 by ymanfryd         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef HTTP_WEBSERVER_REQUEST_PARSER_HPP
#define HTTP_WEBSERVER_REQUEST_PARSER_HPP
#include "../../inc/webserv.hpp"
#include <string>

class RequestParser {

    private:
            std::string                                 host_;
            std::string                                 body_;
            std::string                                 route_;
            std::string                                 query_;
            std::string                                 accept_;
            std::string                                 method_;
            std::string                                 request_;
            std::string                                 pathInfo_;
            std::string                                 protocol_;
            std::string                                 userAgent_;
            std::string                                 connection_;
            std::string                                 contentType_;
            std::string                                 secFetchDest_;
            std::string                                 secFetchMode_;
            std::string                                 secFetchSite_;
            std::string                                 secFetchUser_;
            std::string                                 cacheControl_;
            std::string                                 acceptLanguage_;
            std::string                                 acceptEncoding_;

            std::vector<std::string>                    path_;
            size_t                                      iterator_;
            size_t                                      contentLength_;
            std::map<std::string, std::string>          headers_;
            std::vector<std::string>                    supportedMethods_;

            void                                        setMethod();
            void                                        setRoute();
            void                                        setPath();
            void                                        setProtocol();
            void                                        setHost();
            void                                        setUserAgent();
            void                                        setAccept();
            void                                        setAcceptLanguage();
            void                                        setAcceptEncoding();
            void                                        setConnection();
            void                                        setSecFetchDest();
            void                                        setSecFetchMode();
            void                                        setSecFetchSite();
            void                                        setSecFetchUser();
            void                                        setCacheControl();
            void                                        setBody();
            void                                        setHeaders();
            void                                        setContentLength();
            void                                        setContentType();

            void                                        parse();
            void                                        setSupportedMethods();
            std::string                                 parseByChar(const std::string& string, char symbol);
            std::string                                 parseByHeaderName(const std::string& name);
            static std::string                          EraseSpaces(const std::string& string);

public:
            RequestParser();
            RequestParser(std::string request, size_t requestLen);
            RequestParser(const RequestParser &other);
            RequestParser &operator=(const RequestParser &other);
            ~RequestParser() {}

            void                                        setPathInfo(std::string pathInfo);

            const   std::string                         &getRequest()            const;
            const   std::string                         &getMethod()             const;
            const   std::string                         &getRoute()              const;
            const   std::string                         &getQuery()              const;
            const   std::string                         &getProtocol()           const;
            const   std::string                         &getHost()               const;
            const   std::string                         &getUserAgent()          const;
            const   std::string                         &getAccept()             const;
            const   std::string                         &getAcceptLanguage()     const;
            const   std::string                         &getAcceptEncoding()     const;
            const   std::string                         &getConnection()         const;
            const   std::string                         &getSecFetchDest()       const;
            const   std::string                         &getSecFetchMode()       const;
            const   std::string                         &getSecFetchSite()       const;
            const   std::string                         &getSecFetchUser()       const;
            const   std::string                         &getCacheControl()       const;
            const   std::string                         &getBody()               const;
            const   std::string                         &getPathInfo()           const;
            const   std::map<std::string, std::string>  &getHeaders()            const;
            const   std::string                         &getContentType()        const;
            const   size_t                              &getContentLength()      const;
            const   std::vector<std::string>            &getPath()               const;
            const   std::vector<std::string>            &getSupportedMethods()   const;

            void                                        showHeaders();
            bool                                        isSupportedMethod();

            class UnsupportedMethodException: public std::exception
            {
                private:
                        std::string         message_;
                public:
                        explicit UnsupportedMethodException(std::string const& method);
                        const char* what() const throw();
                        virtual ~UnsupportedMethodException() throw() {}
            };

};

#endif //HTTP_WEBSERVER_REQUEST_PARSER_HPP
