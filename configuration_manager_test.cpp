#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <memory>
#include "configuration_manager.h"

#include <fstream>

using ::testing::Return;
using ::testing::_;

TEST(ConfigurationManager, ParseConfigurationWithReadingFile)
{
    ConfigurationManager manager("", "");
    auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_FILE_EMPTY);
}

TEST(ConfigurationManager, ParseConfigurationWithInvalidJSON)
{
    ConfigurationManager manager("", "");

    const std::string invalid_json = R"({"key": "value")";
    manager.set_raw_config(invalid_json);
    const auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::ERR_CONFIGURATION_FILE_NOT_PARSEABLE);
}

TEST(ConfigurationManager, ParseConfigurationWithValidJSONButInvalidSchema)
{
    ConfigurationManager manager("", "");

    const std::string valid_json = R"({"key": "value"})";
    manager.set_raw_config(valid_json);
    const auto result = manager.parse_configuration();

    EXPECT_EQ(result, PingerResult::OK);
}
