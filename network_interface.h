#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <stdexcept>
#include <curlcpp/curl_easy.h>
#include <nlohmann/json.hpp>

#include "errors.h"
#include "jsonable.h"

class INetworkInterface {
public:
    void send_packet() {
        throw std::runtime_error("send_packet must be implemented by the derived class");
    }
};

class NetworkInterface : public INetworkInterface {
public:
    template <typename T>
        requires std::is_base_of_v<Jsonable, T>
    PingerResult send_packet(T packet, const std::string& url);
};

#endif //NETWORK_INTERFACE_H
