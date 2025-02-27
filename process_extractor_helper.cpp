#include <memory>
#include <string>

#include "process_extractor_helper.h"

ProcessExtractorHelpers::ProcessExtractorHelpers(ILogger& logger, ICommandExecutor& command_executor): _logger(logger), _command_executor(command_executor) {
}

bool ProcessExtractorHelpers::get_process_is_running(const std::string &process_name) {
    // Use 'ps' command to find the process ID by name
    const std::string cmd = "ps -C " + process_name + " -o pid=,stat=,comm=";
    const std::string output = _command_executor.exec(cmd.c_str());

    return !output.empty();
}
