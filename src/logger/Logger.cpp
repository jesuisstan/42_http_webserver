#include "Logger.hpp"


void        Logger::printCriticalMessage() {
    if (DEBUG == 0) {
        pthread_mutex_lock(&g_write);
        std::cout << message_.str() << RESET << std::endl;
        pthread_mutex_unlock(&g_write);
        message_.clear();
    }
}
void        Logger::printInfoMessage() {
    if (DEBUG == 1) {
        pthread_mutex_lock(&g_write);
        std::cout << message_.str() << RESET << std::endl;
        pthread_mutex_unlock(&g_write);
        message_.clear();
    }
}

void        Logger::printDebugMessage() {
    if (DEBUG > 1) {
        pthread_mutex_lock(&g_write);
        std::cout << message_.str() << RESET << std::endl;
        pthread_mutex_unlock(&g_write);
        message_.clear();
    }
}