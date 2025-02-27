#include <memory>
#include <string>
#include <cstdio>
#include <array>
#include <stdexcept>
#include "process_extractor_helper.h"

ProcessExtractorHelpers::ProcessExtractorHelpers(ILogger& logger): _logger(logger) {}

std::string ProcessExtractorHelpers::exec(const char *cmd) {
    std::array<char, 2048> buffer{};
    std::string result;
    const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}

std::string ProcessExtractorHelpers::get_service_status(const std::string &process_name) {
    // Use 'ps' command to find the process ID by name
    const std::string cmd = "ps -C " + process_name + " -o pid=,stat=,comm=";
    const std::string output = exec(cmd.c_str());

    if (output.empty()) {
        _logger.log(LOG_ERR, "Service not running or process not found.");
        return "";
    }

    // Parse the output to determine the status
    std::string status;
    if (const size_t pos = output.find_first_not_of(" \t\n"); pos != std::string::npos) {
        if (const size_t statusPos = output.find_first_of(" \t\n", pos); statusPos != std::string::npos) {
            status = output.substr(statusPos + 1, 1); // Extract the status code (e.g., S, R, Z, etc.)
        }
    }

    // Map the status code to a human-readable status
    if (status == "S") {
        return "Running (Sleeping)";
    } else if (status == "R") {
        return "Running";
    } else if (status == "Z") {
        return "Zombie";
    } else if (status == "T") {
        return "Stopped";
    } else {
        return "Unknown status";
    }
}
