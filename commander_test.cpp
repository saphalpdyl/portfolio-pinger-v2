#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include "commander.h"

// Create mock classes for dependencies
class MockLogger final : public ILogger {
public:
    MOCK_METHOD(void, log, (int level, const std::string& message), (override));
};

class MockServiceManager final : public IServiceManager {
public:
    MOCK_METHOD(bool, install_service, (), (override));
    MOCK_METHOD(bool, uninstall_service, (), (override));
    MOCK_METHOD(bool, run_service, (), (override));
};

class CommanderTest : public ::testing::Test {
protected:
    void SetUp() override {
        // Create the Commander instance in setup
        commander = std::make_unique<Commander>(logger, service_manager);
    }

    void TearDown() override {
        // Clean up if needed
    }

    MockLogger logger;
    MockServiceManager service_manager;
    std::unique_ptr<Commander> commander;
};

// Test with no arguments
TEST_F(CommanderTest, NoArgumentsReturnsFailure) {
    // Arrange
    const char* argv[] = {"program_name"};
    constexpr int argc = 1;

    // Expect the logger to be called with error message
    testing::Mock::AllowLeak(&logger);
    EXPECT_CALL(logger, log(LOG_ERR, "No command provided"))
        .Times(1);

    // Act
    const bool result = commander->process_arguments(argc, const_cast<char**>(argv));

    // Assert
    EXPECT_FALSE(result);
}

// Test with install command
TEST_F(CommanderTest, InstallCommandCallsServiceManager) {
    // Arrange
    const char* argv[] = {"program_name", "install"};
    constexpr int argc = 2;

    // Expect install_service to be called once and return true
    EXPECT_CALL(service_manager, install_service())
        .Times(1)
        .WillOnce(::testing::Return(true));

    // Act
    const bool result = commander->process_arguments(argc, const_cast<char**>(argv));

    // Assert
    EXPECT_TRUE(result);
}

// Test with uninstall command
TEST_F(CommanderTest, UninstallCommandCallsServiceManager) {
    // Arrange
    const char* argv[] = {"program_name", "uninstall"};
    constexpr int argc = 2;

    // Expect uninstall_service to be called once and return true
    EXPECT_CALL(service_manager, uninstall_service())
        .Times(1)
        .WillOnce(::testing::Return(true));

    // Act
    const bool result = commander->process_arguments(argc, const_cast<char**>(argv));

    // Assert
    EXPECT_TRUE(result);
}

// Test with run command
TEST_F(CommanderTest, RunCommandCallsServiceManager) {
    // Arrange
    const char* argv[] = {"program_name", "run"};
    constexpr int argc = 2;

    // Expect run_service to be called once and return true
    EXPECT_CALL(service_manager, run_service())
        .Times(1)
        .WillOnce(::testing::Return(true));

    // Act
    const bool result = commander->process_arguments(argc, const_cast<char**>(argv));

    // Assert
    EXPECT_TRUE(result);
}

// Test with unknown command
TEST_F(CommanderTest, UnknownCommandReturnsFailure) {
    // Arrange
    const char* argv[] = {"program_name", "invalid_command"};
    constexpr int argc = 2;

    // Expect the logger to be called with error message
    EXPECT_CALL(logger, log(LOG_ERR, "Unknown command: invalid_command"))
        .Times(1);

    // Act
    const bool result = commander->process_arguments(argc, const_cast<char **>(argv));

    // Assert
    EXPECT_FALSE(result);
}

// Test with service manager returning false
TEST_F(CommanderTest, ServiceManagerFailureIsProperlyReturned) {
    // Arrange
    const char* argv[] = {"program_name", "install"};
    constexpr int argc = 2;

    // Expect install_service to be called once and return false
    EXPECT_CALL(service_manager, install_service())
        .Times(1)
        .WillOnce(::testing::Return(false));

    // Act
    const bool result = commander->process_arguments(argc, const_cast<char**>(argv));

    // Assert
    EXPECT_FALSE(result);
}