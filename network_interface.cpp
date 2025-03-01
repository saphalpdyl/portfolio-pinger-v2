#include "network_interface.h"

#include "process_payload.h"
#include "process_target_result.h"

using curl::curl_easy;
using curl::curl_easy_exception;
using curl::curlcpp_traceback;

using nlohmann::json;

template PingerResult NetworkInterface::send_packet<ProcessTargetResult>(ProcessTargetResult, const std::string&);
template PingerResult NetworkInterface::send_packet<ProcessPayload>(ProcessPayload, const std::string&);

template<typename T> requires std::is_base_of_v<Jsonable, T>
PingerResult NetworkInterface::send_packet(T packet, const std::string &url) {
    try {
        curl_easy easy;

        easy.add<CURLOPT_URL>(url.c_str());
        easy.add<CURLOPT_POST>(1L);

        // Adding headers
        curl_slist* headers = nullptr;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        easy.add<CURLOPT_HTTPHEADER>(headers);

        auto body = packet.to_json().dump();
        easy.add<CURLOPT_POSTFIELDS>(body.c_str());

        easy.perform();
    } catch ( curl_easy_exception& error ) {
        curlcpp_traceback errors = error.get_traceback();
        return PingerResult::ERR_NETWORK_INTERFACE_GENERIC;
    }

    return PingerResult::OK;
}
