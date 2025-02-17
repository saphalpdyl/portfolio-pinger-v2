#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <string>
#include <nlohmann/json.hpp>

#include "errors.h"

using namespace nlohmann;

class ConfigurationManager {
public:
    ConfigurationManager(const std::string& config_file_name, const std::string& config_dir);

    PingerResult load_configuration() const;

private:
    std::string _config_file_name;
    std::string _config_dir;
};


#endif //CONFIGURATIONMANAGER_H
