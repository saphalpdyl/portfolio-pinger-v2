#ifndef PROCESS_PAYLOAD_H
#define PROCESS_PAYLOAD_H

#include <string>
#include <vector>
#include <nlohmann/json.hpp>

#include "jsonable.h"
#include "process_target_result.h"

using nlohmann::json;

class ProcessPayload final: Jsonable {

public:
    std::string hash;
    std::vector<ProcessTargetResult> target_results;

    [[nodiscard]] json get_results_json() const {
        json results_json = json::array();
        for (const auto& result : target_results) {
            results_json.push_back(result.to_json());
        }

        return results_json;
    }

    [[nodiscard]] json to_json() const override {
        return json{
            {"hash", hash},
            {"processes", get_results_json()}
        };
    }
};

#endif //PROCESS_PAYLOAD_H
