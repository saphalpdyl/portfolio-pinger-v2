#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <stdexcept>
#include <curlcpp/curl_easy.h>
#include <nlohmann/json.hpp>

#include "errors.h"
#include "jsonable.h"

class INetworkInterface {
public:
    // Workaround for virtual template method
    // Credits to: https://stackoverflow.com/a/38719807
    template <typename T>
        requires std::is_base_of_v<Jsonable, T>
    PingerResult send_packet(T packet, const std::string& url) {
        return this->send_packet<T>(packet, url);
    };
};

class NetworkInterface : public INetworkInterface {
public:
    template <typename T>
        requires std::is_base_of_v<Jsonable, T>
    PingerResult send_packet(T packet, const std::string& url);
};

#endif //NETWORK_INTERFACE_H
