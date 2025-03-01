#include "webhook_server.h"

#include <openssl/sha.h>
#include <openssl/hmac.h>

#include <iomanip>

#include "process_payload.h"

template PingerResult WebhookServer<NetworkInterface>::send_process_results(std::vector<ProcessTargetResult>&);

template <typename NI>
PingerResult WebhookServer<NI>::send_process_results(std::vector<ProcessTargetResult> &results) {
    ProcessPayload payload{};
    payload.target_results = results;

    const auto processes_json = payload.get_results_json();
    const auto hash = create_hash_from_json_string(processes_json.dump());

    payload.hash = hash;

    _network_interface.template send_packet<ProcessPayload>(payload, _url);
    return PingerResult::OK;
}

// Referenced from https://stackoverflow.com/a/72065940
// Credit to https://stackoverflow.com/users/925913/andrew-cheong
template <typename NI>
std::string WebhookServer<NI>::create_hash_from_json_string(const std::string json_data) {
    std::array<unsigned char, EVP_MAX_MD_SIZE> hash{};
    unsigned int hashLen;

    HMAC(
        EVP_sha256(),
        _hmac_hash_key.data(),
        static_cast<int>(_hmac_hash_key.size()),
        reinterpret_cast<unsigned char const*>(json_data.data()),
        static_cast<int>(json_data.size()),
        hash.data(),
        &hashLen
    );

    // Convert hash to hex string
    std::stringstream out;
    for (unsigned int i=0; i < hashLen; i++) {
        out << std::setfill('0') << std::setw(2) << std::right << std::hex << static_cast<int>(hash[i]);
    }
    return out.str();
}
