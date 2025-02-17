//
// Created by saphalpdyl on 2/17/25.
//

#include "ConfigurationManager.h"
#include <fstream>


ConfigurationManager::ConfigurationManager(
    const std::string& configurationFilename,
    const std::string& configurationDirectory
) {
    _configurationFilename = configurationFilename;
    _configurationDirectory = configurationDirectory;
}


PingerResult ConfigurationManager::loadConfiguration() const {
    // cF = configuration file
    std::ifstream cF(_configurationFilename);

    if ( !cF.is_open() ) {
        return PingerResult::ERR_CONFIGURATION_FILE_NOT_FOUND;
    }

    json data = json::parse(cF);

    return PingerResult::OK;
}
