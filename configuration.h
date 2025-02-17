#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>
#include "process_target.h"

struct Configuration {
    std::string target_url;
    std::string authorization_secret;
    int ping_interval;

    std::vector<ProcessTarget> process_targets;
};

#endif //CONFIGURATION_H
