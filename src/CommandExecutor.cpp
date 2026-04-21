#include "CommandExecutor.h"
#include "Logger.h"
#include <array>
#include <memory>
#include <cstdio>

// Cross-platform popen/pclose macros
#ifdef _WIN32
#define POPEN _popen
#define PCLOSE _pclose
#else
#define POPEN popen
#define PCLOSE pclose
#endif

std::string CommandExecutor::execute(const std::string& command) {
    Logger::info("Executing command: " + command);
    std::array<char, 256> buffer;
    std::string result;
    
    // Using standard library popen to execute system commands
    // Warning: Security relies on the prompt effectively locking out injection.
    std::unique_ptr<FILE, decltype(&PCLOSE)> pipe(POPEN(command.c_str(), "r"), PCLOSE);
    
    if (!pipe) {
        Logger::error("Failed to open pipe for command: " + command);
        throw CommandExecutionException("popen() failed! Could not execute command.");
    }
    
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    
    Logger::info("Command execution completed.");
    return result;
}
