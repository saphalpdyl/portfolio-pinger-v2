#include "configuration_manager.h"
#include <fstream>
#include <iostream>

#include "constants.h"


ConfigurationManager::ConfigurationManager(
    const std::string& config_file_name = CONFIGURATION_FILENAME,
    const std::string& config_dir = CONFIGURATION_DIRECTORY
) {
    _config_file_name = config_file_name;
    _config_dir = config_dir;
}


PingerResult ConfigurationManager::load_configuration() const {
    // cF = configuration file
    std::ifstream cF(_config_dir + "/" + _config_file_name);

    if ( !cF.is_open() ) {
        std::cerr << "Configuration file not found" << std::endl;
        return PingerResult::ERR_CONFIGURATION_FILE_NOT_FOUND;
    }

    try
    {
        json data = json::parse(cF);
    } catch (json::parse_error& e) {
        std::cerr << "Configuration file not parseable\n" << e.what() << std::endl;
        return PingerResult::ERR_CONFIGURATION_FILE_NOT_PARSEABLE;
    }

    return PingerResult::OK;
}