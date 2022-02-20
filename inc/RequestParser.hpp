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
#include "webserv.hpp"

class RequestParser {

    private:
            std::string                 request_;
            std::string                 method_;
            std::string                 route_;
            std::string                 protocol_;
            std::string                 host_;
            std::string                 userAgent_;
            std::string                 accept_;
            std::string                 acceptLanguage_;
            std::string                 acceptEncoding_;
            std::string                 connection_;
            std::string                 secFetchDest_;
            std::string                 secFetchMode_;
            std::string                 secFetchSite_;
            std::string                 secFetchUser_;
            std::string                 cacheControl_;
            std::string                 headers_;
            std::string                 body_;
            size_t                      contentLength_;

            size_t                      iterator_;
            std::vector<std::string>    supportedMethods_;

            void                        setMethod();
            void                        setRoute();
            void                        setProtocol();
            void                        setHost();
            void                        setUserAgent();
            void                        setAccept();
            void                        setAcceptLanguage();
            void                        setAcceptEncoding();
            void                        setConnection();
            void                        setSecFetchDest();
            void                        setSecFetchMode();
            void                        setSecFetchSite();
            void                        setSecFetchUser();
            void                        setCacheControl();
            void                        setBody();
            void                        setHeaders();
            void                        setContentLength();

            void                        parse();
            void                        setSupportedMethods();
            std::string                 parseByChar(const std::string& string, char symbol);
            std::string                 parseByHeaderName(const std::string& name);
            static std::string          EraseSpaces(const std::string& string);

public:

            RequestParser(std::string request, size_t requestLen);
            RequestParser(const RequestParser &other);
            RequestParser &operator=(const RequestParser &other);
            ~RequestParser() {}

            std::string getRequest()        const;
            std::string getMethod()         const;
            std::string getRoute()          const;
            std::string getProtocol()       const;
            std::string getHost()           const;
            std::string getUserAgent()      const;
            std::string getAccept()         const;
            std::string getAcceptLanguage() const;
            std::string getAcceptEncoding() const;
            std::string getConnection()     const;
            std::string getSecFetchDest()   const;
            std::string getSecFetchMode()   const;
            std::string getSecFetchSite()   const;
            std::string getSecFetchUser()   const;
            std::string getCacheControl()   const;
            std::string getBody()           const;
            std::string getHeaders()        const;
            size_t      getContentLength()  const;

            bool        isSupportedMethod();

            class UnsupportedMethodException: public std::exception
            {
                private:
                        std::string         message_;
                public:
                        explicit UnsupportedMethodException(std::string const& method);
                        const char* what() const throw();
                        virtual ~UnsupportedMethodException() throw() {}
            };

    RequestParser();
};

#endif //HTTP_WEBSERVER_REQUEST_PARSER_HPP
