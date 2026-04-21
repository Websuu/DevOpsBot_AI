#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <iostream>
#include <mutex>

/// @brief A thread-safe logger for application events.
class Logger {
public:
    /// @brief Writes an info message to stdout.
    /// @param msg The message to log.
    static void info(const std::string& msg);

    /// @brief Writes an error message to stderr.
    /// @param msg The error message to log.
    static void error(const std::string& msg);

private:
    static std::mutex logMutex;
};

#endif // LOGGER_H
