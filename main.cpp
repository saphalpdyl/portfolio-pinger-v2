#include "commander.h"
#include "command_executor.h"
#include "configuration_manager.h"
#include "network_interface.h"
#include "process_extractor_helper.h"
#include "service_manager.h"

// TODOS:
// [x] Create a configuration loading system
// [x] Handle network requests and errors
// [x] Handle HMAC hashing
// [x] Retrieve open application data from the system
// [x] Daemonize the code

int main(const int argc, char* argv[]) {
    Logger std_logger(LoggerMode::CONSOLE);
    Logger logger(LoggerMode::SYSLOG);

    CommandExecutor command_executor(logger);

    const std::string home_dir = CONFIGURATION_HOME_DIRECTORY;

    ConfigurationManager configuration_manager(logger, "pinger.config.json", home_dir);
    ServiceManager service_manager(std_logger, command_executor, configuration_manager);

    const Commander commander(std_logger, service_manager);
    commander.process_arguments(argc, argv);

    return 0;
}