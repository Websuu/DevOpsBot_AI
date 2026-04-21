#include "Logger.h"

std::mutex Logger::logMutex;

void Logger::info(const std::string& msg) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::cout << "[INFO] " << msg << std::endl;
}

void Logger::error(const std::string& msg) {
    std::lock_guard<std::mutex> lock(logMutex);
    std::cerr << "[ERROR] " << msg << std::endl;
}
