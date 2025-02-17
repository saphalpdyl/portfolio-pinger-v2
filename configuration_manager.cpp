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

PingerResult ConfigurationManager::load_file()
{
    // cF = configuration file
    std::stringstream ss;
    std::ifstream cF(_config_dir + "/" + _config_file_name);

    if ( !cF.is_open() ) {
        std::cerr << "Configuration file not found" << std::endl;
        return PingerResult::ERR_CONFIGURATION_FILE_NOT_FOUND;
    }

    ss << cF.rdbuf();
    _raw_config = ss.str();

    return PingerResult::OK;
}

PingerResult ConfigurationManager::parse_configuration()
{
    if ( _raw_config.empty() )
    {
        std::cerr << "Config file is empty: config was likely not loaded" << std::endl;
        return PingerResult::ERR_CONFIGURATION_FILE_EMPTY;
    }

    try
    {
        json data = json::parse(_raw_config);
    } catch (json::parse_error& e) {
        std::cerr << "Configuration file not parseable\n" << e.what() << std::endl;
        return PingerResult::ERR_CONFIGURATION_FILE_NOT_PARSEABLE;
    }

    return PingerResult::OK;
}
