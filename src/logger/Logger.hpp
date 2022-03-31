//
// Created by Yellowjacket Manfryd on 3/30/22.
//

#ifndef HTTP_WEBSERVER_LOGGER_HPP
#define HTTP_WEBSERVER_LOGGER_HPP
#include "../../inc/webserv.hpp"

extern pthread_mutex_t g_write;

class Logger {

    public:

            Logger() {}
            ~Logger() {}

            static void printCriticalMessage(std::stringstream *message_);
            static void printInfoMessage(std::stringstream *message_);
            static void printDebugMessage(std::stringstream *message_);
};


#endif //HTTP_WEBSERVER_LOGGER_HPP
