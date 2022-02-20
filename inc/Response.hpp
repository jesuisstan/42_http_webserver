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
            std::map<int, std::string>  responseCodes_;

            void                        setResponseCodes();
            void                        createResponse();
            void                        setResponse(std::string response);
            void                        homeRoot();
            std::string                 countContentLength();

            Response() {};
    public:

            std::string getResponse();

            explicit Response(RequestParser &request);
            Response(const Response &other);
            Response &operator=(const Response &other);
            ~Response() {}
};

#endif //HTTP_WEBSERVER_RESPONSE_HPP
