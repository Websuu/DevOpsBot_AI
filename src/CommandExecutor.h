#ifndef COMMANDEXECUTOR_H
#define COMMANDEXECUTOR_H

#include <string>
#include <stdexcept>

/// @brief Exception thrown when a system command fails to execute.
class CommandExecutionException : public std::runtime_error {
public:
    explicit CommandExecutionException(const std::string& msg) : std::runtime_error(msg) {}
};

/// @brief Executes system commands securely and retrieves their output.
class CommandExecutor {
public:
    /// @brief Executes a shell command and returns the standard output.
    /// @param command The system command to execute.
    /// @return The standard output of the command.
    /// @throws CommandExecutionException if popen fails.
    static std::string execute(const std::string& command);
};

#endif // COMMANDEXECUTOR_H
