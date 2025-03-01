#ifndef NETWORK_INTERFACE_H
#define NETWORK_INTERFACE_H

#include <nlohmann/json.hpp>

#include "errors.h"
#include "jsonable.h"

template <typename D>
class INetworkInterface {
public:
    // Workaround for virtual template method
    // CRTP: Curiously Recurring Template Pattern
    // Credits to: https://stackoverflow.com/a/38719807
    template <typename T>
        requires std::is_base_of_v<Jsonable, T>
    PingerResult send_packet(T packet, const std::string& url) {
        return static_cast<D *>(this)->template send_packet_impl<T>(packet, url);
    };
};

class NetworkInterface : public INetworkInterface<NetworkInterface> {
public:
    template <typename T>
        requires std::is_base_of_v<Jsonable, T>
    PingerResult send_packet_impl(T packet, const std::string& url);
};

#endif //NETWORK_INTERFACE_H
