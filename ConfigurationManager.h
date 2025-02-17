//
// Created by saphalpdyl on 2/17/25.
//

#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <fstream>
#include <string>
#include <nlohmann/json.hpp>

#include "errors.h"

using namespace nlohmann;

class ConfigurationManager {
    ConfigurationManager(
        const std::string& configurationFilename,
        const std::string& configurationDirectory
    ) {
        _configurationFilename = configurationFilename;
        _configurationDirectory = configurationDirectory;
    }

    PingerResult loadConfiguration() {
        // cF = configuration file
        std::ifstream cF(_configurationFilename);

        if ( !cF.is_open() ) {
            return PingerResult::ERR_CONFIGURATION_FILE_NOT_FOUND;
        }

        json data = json::parse(cF);

        return PingerResult::OK;
    }

private:
    std::string _configurationFilename;
    std::string _configurationDirectory;
};


#endif //CONFIGURATIONMANAGER_H
