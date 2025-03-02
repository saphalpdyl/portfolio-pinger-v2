#ifndef WEBHOOK_SERVER_H
#define WEBHOOK_SERVER_H

#include <nlohmann/json.hpp>
#include <utility>

#include "configuration_manager.h"
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

template <typename NI>
class WebhookServer final: public IWebhookServer {
public:
    PingerResult send_process_results(std::vector<ProcessTargetResult> &results) override;

    explicit WebhookServer(ILogger& logger, INetworkInterface<NI>& network_interface, IConfigurationManager& configuration_manager)
        : _logger(logger), _network_interface(network_interface), _configuration_manager(configuration_manager) {
    }

private:
    std::string create_hash_from_json_string(std::string json_data) override;

    ILogger& _logger;
    INetworkInterface<NI> & _network_interface;
    IConfigurationManager& _configuration_manager;
};



#endif //WEBHOOK_SERVER_H
