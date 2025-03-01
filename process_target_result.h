#ifndef PROCESS_TARGET_RESULT_H
#define PROCESS_TARGET_RESULT_H

#include "process_target.h"
#include "jsonable.h"

#include <nlohmann/json.hpp>

using nlohmann::json;

struct ProcessTargetResult final : ProcessTarget, public Jsonable {
    std::string application_name;
    std::string process_name;
    bool is_running{};

    [[nodiscard]] json to_json() const override {
        return json{
            {"application_name", application_name},
            {"process_name", process_name},
            {"is_running", is_running}
        };
    }
};

#endif //PROCESS_TARGET_RESULT_H
