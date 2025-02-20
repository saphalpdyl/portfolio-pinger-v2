#include "configuration_manager.h"
#include <fstream>

#include "constants.h"

ConfigurationManager::ConfigurationManager(
    ILogger& logger,
    const std::string& config_file_name ,
    const std::string& config_dir
) : _logger(logger)
{
    _config_file_name = config_file_name;
    _config_dir = config_dir;
    _config = nullptr;
}

ConfigurationManager::ConfigurationManager(ILogger &logger): _logger(logger) {
    _config_file_name = CONFIGURATION_FILENAME;
    _config_dir = CONFIGURATION_DIRECTORY;
    _config = nullptr;
}

PingerResult ConfigurationManager::load_file()
{
    std::stringstream ss;
    std::ifstream cF(_config_dir + "/" + _config_file_name);

    if (!cF.is_open()) {
        _logger.log(LOG_ERR, "Configuration file not found");
        return PingerResult::ERR_CONFIGURATION_FILE_NOT_FOUND;
    }

    ss << cF.rdbuf();
    _raw_config = ss.str();

    return PingerResult::OK;
}

PingerResult ConfigurationManager::deserialize_configuration()
{
    if (_raw_config.empty()) {
        _logger.log(LOG_ERR, "Config file is empty: config was likely not loaded");
        return PingerResult::ERR_CONFIGURATION_FILE_EMPTY;
    }

    try {
        _serialized_config = std::make_unique<json>(json::parse(_raw_config));
    } catch (json::parse_error& e) {
        _logger.log(LOG_ERR, "Configuration file not parseable: " + std::string(e.what()));
        return PingerResult::ERR_CONFIGURATION_FILE_NOT_PARSEABLE;
    }

    return PingerResult::OK;
}

PingerResult ConfigurationManager::parse_configuration()
{
    if (_raw_config.empty()) {
        _logger.log(LOG_ERR, "Config file is empty");
        return PingerResult::ERR_CONFIGURATION_FILE_EMPTY;
    }

    if (_serialized_config == nullptr) {
        _logger.log(LOG_ERR, "Configuration not loaded");
        return PingerResult::ERR_CONFIGURATION_NOT_LOADED;
    }

    try {
        auto serialized_config = *_serialized_config;

        _config = std::make_shared<Configuration>();

        if (serialized_config.contains("target_url") && serialized_config["target_url"].is_string())
            _config->target_url = serialized_config["target_url"].get<std::string>();
        else {
            _logger.log(LOG_ERR, "Missing target_url in configuration");
            return PingerResult::ERR_CONFIGURATION_MISSING_TARGET_URL;
        }

        if (serialized_config.contains("authorization_secret") && serialized_config["authorization_secret"].is_string())
            _config->authorization_secret = serialized_config["authorization_secret"].get<std::string>();
        else {
            _logger.log(LOG_ERR, "Missing authorization_secret in configuration");
            return PingerResult::ERR_CONFIGURATION_MISSING_AUTH_SECRET;
        }

        if (serialized_config.contains("ping_interval") && serialized_config["ping_interval"].is_number_integer())
            _config->ping_interval = serialized_config["ping_interval"].get<int>();
        else {
            _logger.log(LOG_ERR, "Missing ping_interval in configuration");
            return PingerResult::ERR_CONFIGURATION_MISSING_PING_INTERVAL;
        }

        // Parse process targets array
        if (serialized_config.contains("process_targets") && serialized_config["process_targets"].is_array()) {
            for (const auto& target : serialized_config["process_targets"]) {
                ProcessTarget process_target;

                if (target.contains("app_name") && target["app_name"].is_string())
                    process_target.application_name = target["app_name"].get<std::string>();
                else {
                    _logger.log(LOG_ERR, "Invalid process target: missing or invalid app_name");
                    return PingerResult::ERR_CONFIGURATION_INVALID_PROCESS_TARGET;
                }

                if (target.contains("process_name") && target["process_name"].is_string())
                    process_target.process_name = target["process_name"].get<std::string>();
                else {
                    _logger.log(LOG_ERR, "Invalid process target: missing or invalid process_name");
                    return PingerResult::ERR_CONFIGURATION_INVALID_PROCESS_TARGET;
                }

                _config->process_targets.push_back(process_target);
            }
        } else {
            _logger.log(LOG_ERR, "Missing process_targets array in configuration");
            return PingerResult::ERR_CONFIGURATION_MISSING_PROCESS_TARGETS;
        }
    } catch (json::parse_error& e) {
        _config = nullptr;
        _logger.log(LOG_ERR, "Configuration file not parseable: " + std::string(e.what()));
        return PingerResult::ERR_CONFIGURATION_FILE_NOT_PARSEABLE;
    } catch (std::exception& e) {
        _config = nullptr;
        _logger.log(LOG_ERR, "Error parsing configuration: " + std::string(e.what()));
        return PingerResult::ERR_CONFIGURATION_GENERIC;
    }

    return PingerResult::OK;
}

std::shared_ptr<Configuration> ConfigurationManager::get_configuration() const
{
    return _config;
}