#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H
#include "command_executor.h"
#include "configuration_manager.h"
#include "logger.h"

const std::string SERVICE_NAME = ENV_SERVICE_NAME;

class IServiceManager {
public:
    virtual ~IServiceManager() = default;

    [[nodiscard]] virtual bool install_service() = 0;
    [[nodiscard]] virtual bool uninstall_service() = 0;
    [[nodiscard]] virtual bool run_service() = 0;
};

class ServiceManager final: public IServiceManager {
public:
    ServiceManager(Logger& logger, CommandExecutor& cmd_exec, ConfigurationManager& cfg_mgr)
        : _logger(logger), _command_executor(cmd_exec), _config_manager(cfg_mgr) {}

private:
    bool install_service() override;
    [[nodiscard]] bool uninstall_service() override;
    [[nodiscard]] bool run_service() override;

    static std::string get_executable_path();

    static bool write_file(const std::string& path, const std::string& content);

    ILogger& _logger;
    ICommandExecutor& _command_executor;
    IConfigurationManager& _config_manager;
};



#endif //SERVICE_MANAGER_H
