#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <string>
#include <nlohmann/json.hpp>
#include <memory>

#include "configuration.h"
#include "errors.h"
#include "logger.h"

using namespace nlohmann;

class ConfigurationManager {
public:
    ConfigurationManager(
        ILogger &logger,
        const std::string &config_file_name,
        const std::string &config_dir
    );

    explicit ConfigurationManager(ILogger &logger);

    PingerResult load_file();
    PingerResult deserialize_configuration();
    PingerResult parse_configuration();
    [[nodiscard]] std::shared_ptr<Configuration> get_configuration() const;

#ifdef TESTING
    inline void set_raw_config(const std::string& raw_config)
    {
        _raw_config = raw_config;
    }
#endif

private:
    std::string _config_file_name;
    std::string _config_dir;
    std::string _raw_config;
    std::unique_ptr<json> _serialized_config;
    std::shared_ptr<Configuration> _config;

    ILogger& _logger;
};


#endif //CONFIGURATIONMANAGER_H
