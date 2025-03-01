#ifndef WEBHOOK_SERVER_H
#define WEBHOOK_SERVER_H

#include <nlohmann/json.hpp>
#include <utility>

#include "logger.h"

using nlohmann::json;

class IWebhookServer {
public:
    virtual ~IWebhookServer() = default;

protected:
    virtual std::string create_hash_from_json_string(std::string json_data) = 0;
};

class WebhookServer final: public IWebhookServer {
public:
    explicit WebhookServer(std::string &url, std::string hmac_key, ILogger& logger)
        : _url(url), _logger(logger), _hmac_hash_key(std::move(hmac_key)) {}

public:
    std::string create_hash_from_json_string(std::string json_data) override;

    std::string& _url;
    ILogger& _logger;
    std::string _hmac_hash_key;
};



#endif //WEBHOOK_SERVER_H
