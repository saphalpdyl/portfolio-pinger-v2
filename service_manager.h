#ifndef SERVICE_MANAGER_H
#define SERVICE_MANAGER_H
#include "command_executor.h"
#include "configuration_manager.h"
#include "logger.h"

const std::string SERVICE_NAME = ENV_SERVICE_NAME;

class DaemonManager {
public:
    DaemonManager(Logger& logger, CommandExecutor& cmd_exec, ConfigurationManager& cfg_mgr)
        : _logger(logger), _command_executor(cmd_exec), _config_manager(cfg_mgr) {}

    bool process_command(const std::string& command);

private:
    bool install_service();
    [[nodiscard]] bool uninstall_service() const;
    bool run_service() const;

    static std::string get_executable_path();

    static bool write_file(const std::string& path, const std::string& content);

    Logger& _logger;
    CommandExecutor& _command_executor;
    ConfigurationManager& _config_manager;
};



#endif //SERVICE_MANAGER_H
