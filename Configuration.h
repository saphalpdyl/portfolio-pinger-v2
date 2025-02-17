//
// Created by saphalpdyl on 2/17/25.
//

#ifndef CONFIGURATION_H
#define CONFIGURATION_H

#include <string>

struct Configuration {
    std::string targetURL;
    std::string authorizationSecret;
    int pingInterval;
};

#endif //CONFIGURATION_H
