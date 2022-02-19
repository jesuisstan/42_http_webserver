//
// Created by Yellowjacket Manfryd on 2/19/22.
//

#ifndef HTTP_WEBSERVER_REQUEST_PARSER_HPP
#define HTTP_WEBSERVER_REQUEST_PARSER_HPP
#include "webserv.hpp"

class RequestParser {
private:
    std::string request_;
    std::string method_;
    std::string route_;
    std::string protocol_;
    std::string host_;
    std::string userAgent_;
    std::string accept_;
    std::string acceptLanguage_;
    std::string acceptEncoding_;
    std::string connection_;
    std::string upgradeInsecureRequests_;
    std::string secFetchDest_;
    std::string secFetchMode_;
    std::string secFetchSite_;
    std::string secFetchUser_;
    std::string cacheControl_;
    std::string body_;

    int iterator_;
    std::vector<std::string> supportedMethods_;

public:
    RequestParser();
    explicit RequestParser(std::string request);
    RequestParser(const RequestParser &other);
    RequestParser &operator=(const RequestParser &other);

    std::string getRequest();
    std::string getMethod();
    std::string getRoute();
    std::string getProtocol();
    std::string getHost();
    std::string getUserAgent();
    std::string getAccept();
    std::string getAcceptLanguage();
    std::string getAcceptEncoding();
    std::string getConnection();
    std::string getUpgradeInsecureRequests();
    std::string getSecFetchDest();
    std::string getSecFetchMode();
    std::string getSecFetchSite();
    std::string getSecFetchUser();
    std::string getCacheControl();
    std::string getBody();

    void        setMethod();
    void        setRoute();
    void        setProtocol();
    void        setHost();
    void        setUserAgent();
    void        setAccept();
    void        setAcceptLanguage();
    void        setAcceptEncoding();
    void        setConnection();
    void        setUpgradeInsecureRequests();
    void        setSecFetchDest();
    void        setSecFetchMode();
    void        setSecFetchSite();
    void        setSecFetchUser();
    void        setCacheControl();
    void        setBody();

    void        setSupportedMethods();
    bool        isSupportedMethod();
    std::string parseByChar(std::string string, char symbol);
    std::string parseByHeaderName(std::string name);
    std::string eraseRequest();

    ~RequestParser();
};

#endif //HTTP_WEBSERVER_REQUEST_PARSER_HPP
