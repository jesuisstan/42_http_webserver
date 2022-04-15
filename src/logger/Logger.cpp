#include "Logger.hpp"

void		Logger::printCriticalMessage(std::stringstream *message_) {
		pthread_mutex_lock(&g_write);
		std::cerr << message_->str() << RESET << std::endl;
		pthread_mutex_unlock(&g_write);
		message_->str("");
}
void		Logger::printInfoMessage(std::stringstream *message_) {
	if (DEBUG > 0) {
		pthread_mutex_lock(&g_write);
		std::cerr << message_->str() << RESET << std::endl;
		pthread_mutex_unlock(&g_write);
		message_->str("");
	}
}

void		Logger::printDebugMessage(std::stringstream *message_) {
	if (DEBUG > 1) {
		pthread_mutex_lock(&g_write);
		std::cerr << message_->str() << RESET << std::endl;
		pthread_mutex_unlock(&g_write);
		message_->str("");
	}
}
