#include<gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "configuration_manager.h"
#include <fstream>

using ::testing::Return;
using ::testing::_;

// Test for parse_configuration with empty config
TEST(ConfigurationManager, ParseConfigurationWithEmptyConfig)
{
    ConfigurationManager manager("", "");
    auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_FILE_EMPTY);
}

// Test for parse_configuration with invalid JSON
TEST(ConfigurationManager, ParseConfigurationWithInvalidJSON)
{
    ConfigurationManager manager("", "");

    const std::string invalid_json = R"({"target_url": "https://example.com")";
    manager.set_raw_config(invalid_json);
    const auto result = manager.deserialize_configuration();

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_FILE_NOT_PARSEABLE);
}

// Test for parse_configuration with missing required fields
TEST(ConfigurationManager, ParseConfigurationWithMissingTargetURL)
{
    ConfigurationManager manager("", "");

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
    manager.set_raw_config(incomplete_json);
    manager.deserialize_configuration();
    const auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_MISSING_TARGET_URL);
}

TEST(ConfigurationManager, ParseConfigurationWithMissingAuthSecret)
{
    ConfigurationManager manager("", "");

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
    manager.set_raw_config(incomplete_json);
    manager.deserialize_configuration();
    const auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_MISSING_AUTH_SECRET);
}

TEST(ConfigurationManager, ParseConfigurationWithMissingPingInterval)
{
    ConfigurationManager manager("", "");

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
    manager.set_raw_config(incomplete_json);
    manager.deserialize_configuration();
    const auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_MISSING_PING_INTERVAL);
}

// Test for parse_configuration with invalid process targets
TEST(ConfigurationManager, ParseConfigurationWithMissingProcessTargets)
{
    ConfigurationManager manager("", "");

    const std::string incomplete_json = R"({
        "target_url": "https://example.com",
        "authorization_secret": "secret123",
        "ping_interval": 1000
    })";
    manager.set_raw_config(incomplete_json);
    manager.deserialize_configuration();
    const auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_MISSING_PROCESS_TARGETS);
}

TEST(ConfigurationManager, ParseConfigurationWithInvalidProcessTarget)
{
    ConfigurationManager manager("", "");

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
    manager.set_raw_config(invalid_target_json);
    manager.deserialize_configuration();
    const auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_INVALID_PROCESS_TARGET);
}

// Test for parse_configuration with valid configuration
TEST(ConfigurationManager, ParseConfigurationWithValidConfiguration)
{
    ConfigurationManager manager("", "");

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
    manager.set_raw_config(valid_json);
    manager.deserialize_configuration();
    const auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::OK);

    // Check that the configuration was correctly parsed
    auto config = manager.get_configuration();
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
TEST(ConfigurationManager, LoadAndParseValidConfiguration)
{
    // Setup: Create a temporary config file
    const std::string temp_dir = "./temp_test_dir";
    const std::string temp_file = "config.json";

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

    // Create manager with the temporary file
    ConfigurationManager manager(temp_file, temp_dir);

    // Test loading
    auto load_result = manager.load_file();
    EXPECT_EQ(load_result, PingerResult::OK);

    // Test deserialization
    auto deserialize_result = manager.deserialize_configuration();
    EXPECT_EQ(deserialize_result, PingerResult::OK);

    // Test parsing
    auto parse_result = manager.parse_configuration();
    EXPECT_EQ(parse_result, PingerResult::OK);

    // Verify the configuration
    auto config = manager.get_configuration();
    ASSERT_NE(config, nullptr);
    EXPECT_EQ(config->target_url, "https://example.com");

    // Cleanup
    std::filesystem::remove_all(temp_dir);
}