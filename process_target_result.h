#ifndef PROCESSTARGETRESULT_H
#define PROCESSTARGETRESULT_H
#include "process_target.h"

struct ProcessTargetResult: ProcessTarget {
    bool is_running{};
};

#endif //PROCESSTARGETRESULT_H
