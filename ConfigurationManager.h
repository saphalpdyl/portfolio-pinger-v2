#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <string>
#include <nlohmann/json.hpp>

#include "errors.h"

using namespace nlohmann;

class ConfigurationManager {
    ConfigurationManager(const std::string& configurationFilename, const std::string& configurationDirectory);

    PingerResult loadConfiguration() const;

private:
    std::string _configurationFilename;
    std::string _configurationDirectory;
};


#endif //CONFIGURATIONMANAGER_H
