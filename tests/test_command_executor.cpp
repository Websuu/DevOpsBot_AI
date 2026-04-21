#include <gtest/gtest.h>
#include "../src/CommandExecutor.h"

/// @brief Test successful execution of a safe command.
TEST(CommandExecutorTest, ExecuteValidCommandReturnsOutput) {
    #ifdef _WIN32
    std::string output = CommandExecutor::execute("echo Hello DevOpsBot");
    #else
    std::string output = CommandExecutor::execute("echo 'Hello DevOpsBot'");
    #endif
    
    // Execution should yield our substring 
    EXPECT_TRUE(output.find("Hello DevOpsBot") != std::string::npos);
}

/// @brief Test exception handling for completely invalid commands (Graceful Handling).
TEST(CommandExecutorTest, ExecuteInvalidCommandHandlesGracefully) {
    // Relying on CommandExecutor to not throw, but capture stderror or empty 
    // unless popen structure fundamentally breaks (which triggers exception).
    EXPECT_NO_THROW({
        std::string res = CommandExecutor::execute("non_existent_command_12345");
        // Output might complain the command was not found
    });
}

// Additional test scaffolding for OllamaClient would be inserted similarly mock-in curl.

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
