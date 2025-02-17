#include "configuration_manager.h"
#include <fstream>


ConfigurationManager::ConfigurationManager(
    const std::string& config_file_name,
    const std::string& config_dir
) {
    _config_file_name = config_file_name;
    _config_dir = config_dir;
}


PingerResult ConfigurationManager::load_configuration() const {
    // cF = configuration file
    std::ifstream cF(_config_file_name);

    if ( !cF.is_open() ) {
        return PingerResult::ERR_CONFIGURATION_FILE_NOT_FOUND;
    }

    json data = json::parse(cF);

    return PingerResult::OK;
}
