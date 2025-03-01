#ifndef WEBHOOK_SERVER_H
#define WEBHOOK_SERVER_H

#include <nlohmann/json.hpp>
#include <utility>

#include "logger.h"
#include "network_interface.h"
#include "process_target_result.h"

using nlohmann::json;

class IWebhookServer {
public:
    virtual ~IWebhookServer() = default;
    virtual PingerResult send_process_results(std::vector<ProcessTargetResult>& results) = 0;

protected:
    virtual std::string create_hash_from_json_string(std::string json_data) = 0;
};

class WebhookServer final: public IWebhookServer {
public:
    PingerResult send_process_results(std::vector<ProcessTargetResult> &results) override;

    explicit WebhookServer(std::string &url, std::string hmac_key, ILogger& logger, INetworkInterface& network_interface)
        : _url(url), _logger(logger), _network_interface(network_interface), _hmac_hash_key(std::move(hmac_key)) {}

private:
    std::string create_hash_from_json_string(std::string json_data) override;

    std::string& _url;
    ILogger& _logger;
    INetworkInterface& _network_interface;
    std::string _hmac_hash_key;
};



#endif //WEBHOOK_SERVER_H
