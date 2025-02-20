#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include <string>
#include <nlohmann/json.hpp>
#include <memory>

#include "configuration.h"
#include "errors.h"
#include "logger.h"

using namespace nlohmann;

class IConfigurationManager {
public:
    IConfigurationManager() = default;
    virtual ~IConfigurationManager() = default;

    [[nodiscard]] virtual std::shared_ptr<Configuration> get_configuration() const = 0;

protected:
    std::shared_ptr<Configuration> _config;
};

class ConfigurationManager final : public IConfigurationManager{
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

    [[nodiscard]] std::shared_ptr<Configuration> get_configuration() const override;

#ifdef TESTING
    void set_raw_config(const std::string& raw_config)
    {
        _raw_config = raw_config;
    }
#endif

private:
    std::string _config_file_name;
    std::string _config_dir;
    std::string _raw_config;
    std::unique_ptr<json> _serialized_config;

    ILogger& _logger;
};


#endif //CONFIGURATIONMANAGER_H
