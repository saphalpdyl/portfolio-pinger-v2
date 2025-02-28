#ifndef PROCESS_TARGET_RESULT_H
#define PROCESS_TARGET_RESULT_H
#include "process_target.h"

struct ProcessTargetResult: ProcessTarget {\
    std::string application_name;
    std::string process_name;
    bool is_running{};
};

#endif //PROCESS_TARGET_RESULT_H
