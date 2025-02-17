#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>

struct Configuration {
    std::string target_url;
    std::string authorization_secret;
    int ping_interval;
};

#endif //CONFIGURATION_H
