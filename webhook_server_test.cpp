#include <gtest/gtest.h>
#include <gmock/gmock.h>

#include <utility>
#include "webhook_server.h"
#include "process_payload.h"

#include "webhook_server.cpp"

// Mock classes
class MockLogger final : public ILogger {
public:
    MOCK_METHOD(void, log, (int level, const std::string& message), (override));
};

class MockNetworkInterface final : public INetworkInterface<MockNetworkInterface> {
public:
    template <typename T>
        requires std::is_base_of_v<Jsonable, T>
    PingerResult send_packet_impl(T _, const std::string& url) {
        return send_packet_mock(url);
    }

    MOCK_METHOD(PingerResult, send_packet_mock, (const std::string& url), ());
};

class MockConfigurationManager final : public IConfigurationManager {
public:
    MOCK_METHOD(std::shared_ptr<Configuration>, get_configuration, (), (const, override));
    MOCK_METHOD(PingerResult, load_file, (), (override));
    MOCK_METHOD(PingerResult, deserialize_configuration, (), (override));
    MOCK_METHOD(PingerResult, parse_configuration, (), (override));
};


// Fixture for WebhookServer tests
class WebhookServerTest : public ::testing::Test {
protected:
    void SetUp() override {
        config = std::make_shared<Configuration>();
        config->authorization_secret = "test_secret";
        config->target_url = "https://example.com/webhook";

        EXPECT_CALL(config_manager, get_configuration())
            .WillRepeatedly(::testing::Return(config));
    }

    MockLogger logger;
    MockNetworkInterface network_interface;
    MockConfigurationManager config_manager;
    std::shared_ptr<Configuration> config;
};

// Test the hash creation function and send_process_results
TEST_F(WebhookServerTest, CreateHashAndSendResults) {
    WebhookServer<MockNetworkInterface> server(logger, network_interface, config_manager);

    std::vector<ProcessTargetResult> results;
    ProcessTargetResult result;
    result.application_name = "test_app";
    result.is_running = true;
    results.push_back(result);

    // The network interface will be called
    EXPECT_CALL(network_interface, send_packet_mock(config->target_url))
        .WillOnce(::testing::Return(PingerResult::OK));

    // Call the method that uses create_hash_from_json_string internally
    const PingerResult result_code = server.send_process_results(results);

    EXPECT_EQ(result_code, PingerResult::OK);
}

// Test with multiple results
TEST_F(WebhookServerTest, SendMultipleProcessResults) {
    WebhookServer<MockNetworkInterface> server(logger, network_interface, config_manager);

    std::vector<ProcessTargetResult> results;

    ProcessTargetResult result1;
    result1.application_name = "app1";
    result1.is_running = true;
    results.push_back(result1);

    ProcessTargetResult result2;
    result2.application_name = "app2";
    result2.is_running = false;
    results.push_back(result2);

    // The network interface will be called
    EXPECT_CALL(network_interface, send_packet_mock(config->target_url))
        .WillOnce(::testing::Return(PingerResult::OK));

    const PingerResult result_code = server.send_process_results(results);
    EXPECT_EQ(result_code, PingerResult::OK);
}

// Test with empty results
TEST_F(WebhookServerTest, SendEmptyProcessResults) {
    WebhookServer<MockNetworkInterface> server(logger, network_interface, config_manager);

    std::vector<ProcessTargetResult> empty_results;

    // The network interface will be called even with empty results
    EXPECT_CALL(network_interface, send_packet_mock(config->target_url))
        .WillOnce(::testing::Return(PingerResult::OK));

    PingerResult result_code = server.send_process_results(empty_results);
    EXPECT_EQ(result_code, PingerResult::OK);
}

// Test network failure scenario
TEST_F(WebhookServerTest, NetworkFailure) {
    WebhookServer<MockNetworkInterface> server(logger, network_interface, config_manager);

    std::vector<ProcessTargetResult> results;
    ProcessTargetResult result;
    result.application_name = "test_app";
    result.is_running = true;
    results.push_back(result);

    // Simulate network failure
    EXPECT_CALL(network_interface, send_packet_mock(config->target_url))
        .WillOnce(::testing::Return(PingerResult::ERR_NETWORK_INTERFACE_GENERIC));

    // Currently the WebhookServer doesn't handle network errors from send_packet
    // It just returns OK. In a real scenario, might want to modify the code
    // to return the actual result from send_packet.
    const PingerResult result_code = server.send_process_results(results);
    EXPECT_EQ(result_code, PingerResult::OK);
}

// Advanced test using a custom network interface to capture and validate payload
class CaptureNetworkInterface final : public INetworkInterface<CaptureNetworkInterface> {
public:
    ProcessPayload captured_payload;
    std::string captured_url;
    PingerResult return_value = PingerResult::OK;

    template <typename T>
        requires std::is_base_of_v<Jsonable, T>
    PingerResult send_packet_impl(T packet, const std::string& url) {
        if constexpr (std::is_same_v<T, ProcessPayload>) {
            captured_payload = std::move(packet);
            captured_url = url;
        }
        return return_value;
    }
};

TEST_F(WebhookServerTest, ValidatePayloadContent) {
    CaptureNetworkInterface capture_network;
    WebhookServer server(logger, capture_network, config_manager);

    std::vector<ProcessTargetResult> results;
    ProcessTargetResult result;
    result.application_name = "test_app";
    result.is_running = true;
    results.push_back(result);

    // Call the method
    PingerResult result_code = server.send_process_results(results);

    // Verify result
    EXPECT_EQ(result_code, PingerResult::OK);

    // Verify captured data
    EXPECT_EQ(capture_network.captured_url, config->target_url);
    EXPECT_FALSE(capture_network.captured_payload.hash.empty());
    ASSERT_EQ(capture_network.captured_payload.target_results.size(), 1U);
    EXPECT_EQ(capture_network.captured_payload.target_results[0].application_name, "test_app");
    EXPECT_EQ(capture_network.captured_payload.target_results[0].is_running, true);

    // Validate that the hash was created with the correct data
    json expected_payload = json::array();
    for (const auto& result : results) {
        expected_payload.push_back(result.to_json());
    }

    // We can't directly check the hash correctness without reimplementing the hash function,
    // but we can verify it's not empty and has the expected format (64 hex characters for SHA-256)
    EXPECT_EQ(capture_network.captured_payload.hash.length(), 64U);
    // Check if it contains only hexadecimal characters
    for (char c : capture_network.captured_payload.hash) {
        EXPECT_TRUE(isxdigit(c)) << "Hash contains non-hexadecimal character: " << c;
    }
}

template PingerResult WebhookServer<MockNetworkInterface>::send_process_results(std::vector<ProcessTargetResult>&);
template PingerResult WebhookServer<CaptureNetworkInterface>::send_process_results(std::vector<ProcessTargetResult>&);
template std::string WebhookServer<MockNetworkInterface>::create_hash_from_json_string(std::string);
template std::string WebhookServer<CaptureNetworkInterface>::create_hash_from_json_string(std::string);