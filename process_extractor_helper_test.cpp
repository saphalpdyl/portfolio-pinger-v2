#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>

#include "logger.h"
#include "process_extractor_helper.h"

using ::testing::Return;
using ::testing::_;

// Mock logger class
class MockLogger final : public ILogger {
public:
    explicit MockLogger(const LoggerMode mode) {
        _mode = mode;
    }

    MOCK_METHOD(void, log, (int log_level, const std::string& message), (override));
};

class MockCommandExecutor final : public ICommandExecutor {
public:
    MOCK_METHOD(std::string, exec, (const char* cmd), (override));
};

class ProcessExtractorHelpersTest : public ::testing::Test {
protected:
    void SetUp() override {
        mockLogger = std::make_shared<MockLogger>(LoggerMode::CONSOLE);
        mockCommandExecutor = std::make_shared<MockCommandExecutor>();
        helper = std::make_unique<ProcessExtractorHelpers>(*mockLogger, *mockCommandExecutor);
    }

    void TearDown() override {
        helper.reset();
        mockLogger.reset();
        mockCommandExecutor.reset();
    }

    std::shared_ptr<MockLogger> mockLogger;
    std::shared_ptr<MockCommandExecutor> mockCommandExecutor;
    std::unique_ptr<ProcessExtractorHelpers> helper;
};

// Test case for a running process
TEST_F(ProcessExtractorHelpersTest, GetProcessIsRunning_Running) {
    // Mock exec to return a running process status
    EXPECT_CALL(*mockCommandExecutor, exec(_))
        .WillOnce(Return("1234")); // Simulate a running process

    const bool is_running = helper->get_process_is_running("nginx");
    EXPECT_TRUE(is_running); // Process is running
}

// Test case for a process not found
TEST_F(ProcessExtractorHelpersTest, GetProcessIsRunning_NotFound) {
    // Mock exec to return an empty output (process not found)
    EXPECT_CALL(*mockCommandExecutor, exec(_))
        .WillOnce(Return(""));

    const bool is_running = helper->get_process_is_running("nonexistent");
    EXPECT_FALSE(is_running); // Process is not running
}