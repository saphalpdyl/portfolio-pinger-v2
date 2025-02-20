#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "configuration_manager.h"
#include <fstream>

using ::testing::Return;
using ::testing::_;

// Mock logger class
class MockLogger final : public ILogger {
public:
    explicit MockLogger(const LoggerMode mode) {
        _mode = mode;
    }

    void log( int log_level, const std::string& message) override {
        std::cout << "TEST: " << message << std::endl;
    }
};

// Test fixture class
class ConfigurationManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        logger = std::make_shared<MockLogger>(LoggerMode::CONSOLE);
        manager = std::make_unique<ConfigurationManager>(*logger, "", "");
    }

    void TearDown() override {
        manager.reset();
        logger.reset();
    }

    // Helper method to set configuration and run basic parse steps
    [[nodiscard]] PingerResult set_and_parse_config(const std::string& json_config, bool override_parse_disabled = false) const {
        manager->set_raw_config(json_config);
        const auto serialize_result = manager->deserialize_configuration();
        if (override_parse_disabled) return serialize_result;
        return manager->parse_configuration();
    }

    std::shared_ptr<MockLogger> logger;
    std::unique_ptr<ConfigurationManager> manager;
};

// Test for parse_configuration with empty config
TEST_F(ConfigurationManagerTest, ParseConfigurationWithEmptyConfig) {
    const auto result = manager->parse_configuration();
    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_FILE_EMPTY);
}

// Test for parse_configuration with invalid JSON
TEST_F(ConfigurationManagerTest, DeserializeConfigurationWithInvalidJSON) {

    const std::string invalid_json = R"({"target_url": "https://example.com")";
    const auto result = set_and_parse_config(invalid_json, true);

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_FILE_NOT_PARSEABLE);
}

// Test for parse_configuration with missing required fields
TEST_F(ConfigurationManagerTest, ParseConfigurationWithMissingTargetURL) {

    const std::string incomplete_json = R"({
        "authorization_secret": "secret123",
        "ping_interval": 1000,
        "process_targets": [
            {
                "app_name": "app1",
                "process_name": "process1"
            }
        ]
    })";
    const auto result = set_and_parse_config(incomplete_json);

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_MISSING_TARGET_URL);
}

TEST_F(ConfigurationManagerTest, ParseConfigurationWithMissingAuthSecret) {

    const std::string incomplete_json = R"({
        "target_url": "https://example.com",
        "ping_interval": 1000,
        "process_targets": [
            {
                "app_name": "app1",
                "process_name": "process1"
            }
        ]
    })";
    const auto result = set_and_parse_config(incomplete_json);

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_MISSING_AUTH_SECRET);
}

TEST_F(ConfigurationManagerTest, ParseConfigurationWithMissingPingInterval) {

    const std::string incomplete_json = R"({
        "target_url": "https://example.com",
        "authorization_secret": "secret123",
        "process_targets": [
            {
                "app_name": "app1",
                "process_name": "process1"
            }
        ]
    })";
    const auto result = set_and_parse_config(incomplete_json);

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_MISSING_PING_INTERVAL);
}

TEST_F(ConfigurationManagerTest, ParseConfigurationWithMissingProcessTargets) {

    const std::string incomplete_json = R"({
        "target_url": "https://example.com",
        "authorization_secret": "secret123",
        "ping_interval": 1000
    })";
    const auto result = set_and_parse_config(incomplete_json);

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_MISSING_PROCESS_TARGETS);
}

TEST_F(ConfigurationManagerTest, ParseConfigurationWithInvalidProcessTarget) {

    const std::string invalid_target_json = R"({
        "target_url": "https://example.com",
        "authorization_secret": "secret123",
        "ping_interval": 1000,
        "process_targets": [
            {
                "app_name": "app1"
            }
        ]
    })";
    const auto result = set_and_parse_config(invalid_target_json);

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_INVALID_PROCESS_TARGET);
}

TEST_F(ConfigurationManagerTest, ParseConfigurationWithValidConfiguration) {

    const std::string valid_json = R"({
        "target_url": "https://example.com",
        "authorization_secret": "secret123",
        "ping_interval": 1000,
        "process_targets": [
            {
                "app_name": "app1",
                "process_name": "process1"
            },
            {
                "app_name": "app2",
                "process_name": "process2"
            }
        ]
    })";
    const auto result = set_and_parse_config(valid_json);

    EXPECT_EQ(result, PingerResult::OK);

    // Check that the configuration was correctly parsed
    const auto config = manager->get_configuration();
    ASSERT_NE(config, nullptr);
    EXPECT_EQ(config->target_url, "https://example.com");
    EXPECT_EQ(config->authorization_secret, "secret123");
    EXPECT_EQ(config->ping_interval, 1000);
    ASSERT_EQ(config->process_targets.size(), 2);
    EXPECT_EQ(config->process_targets[0].application_name, "app1");
    EXPECT_EQ(config->process_targets[0].process_name, "process1");
    EXPECT_EQ(config->process_targets[1].application_name, "app2");
    EXPECT_EQ(config->process_targets[1].process_name, "process2");
}

// Integration test that combines loading and parsing
TEST_F(ConfigurationManagerTest, LoadAndParseValidConfiguration) {
    // Create a new manager with actual file paths
    const std::string temp_dir = "./temp_test_dir";
    const std::string temp_file = "config.json";
    auto file_manager = std::make_unique<ConfigurationManager>(*logger, temp_file, temp_dir);

    // Create directory if it doesn't exist
    std::filesystem::create_directories(temp_dir);

    // Write valid JSON to the file
    std::ofstream file(temp_dir + "/" + temp_file);
    file << R"({
        "target_url": "https://example.com",
        "authorization_secret": "secret123",
        "ping_interval": 1000,
        "process_targets": [
            {
                "app_name": "app1",
                "process_name": "process1"
            }
        ]
    })";
    file.close();

    // Test loading
    auto load_result = file_manager->load_file();
    EXPECT_EQ(load_result, PingerResult::OK);

    // Test deserialization
    auto deserialize_result = file_manager->deserialize_configuration();
    EXPECT_EQ(deserialize_result, PingerResult::OK);

    // Test parsing
    auto parse_result = file_manager->parse_configuration();
    EXPECT_EQ(parse_result, PingerResult::OK);

    // Verify the configuration
    auto config = file_manager->get_configuration();
    ASSERT_NE(config, nullptr);
    EXPECT_EQ(config->target_url, "https://example.com");

    // Cleanup
    std::filesystem::remove_all(temp_dir);
}