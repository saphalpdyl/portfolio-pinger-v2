#ifndef PROCESS_TARGET_RESULT_H
#define PROCESS_TARGET_RESULT_H
#include "process_target.h"

struct ProcessTargetResult: ProcessTarget {
    bool is_running{};
};

#endif //PROCESS_TARGET_RESULT_H
