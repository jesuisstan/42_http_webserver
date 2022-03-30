//
// Created by Yellowjacket Manfryd on 3/30/22.
//

#ifndef HTTP_WEBSERVER_LOGGER_HPP
#define HTTP_WEBSERVER_LOGGER_HPP
#include "../../inc/webserv.hpp"

extern pthread_mutex_t g_write;

class Logger {

    public:
            static std::stringstream message_;

            Logger() {}
            ~Logger() {}

            static void printCriticalMessage();
            static void printInfoMessage();
            static void printDebugMessage();
};


#endif //HTTP_WEBSERVER_LOGGER_HPP
