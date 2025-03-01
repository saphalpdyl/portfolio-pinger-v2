#include "network_interface.h"

template<typename T> requires std::is_base_of_v<Jsonable, T>
PingerResult NetworkInterface::send_packet(T packet, const std::string &url) {
    return PingerResult::OK;
}
