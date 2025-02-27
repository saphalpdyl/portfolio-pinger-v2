#include <memory>
#include <string>
#include <cstdio>
#include <array>
#include <stdexcept>

#include "command_executor.h"

std::string CommandExecutor::exec(const char *cmd) {
    std::array<char, 2048> buffer{};
    std::string result;
    const std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        _logger.log(LOG_ERR, "popen() failed!");
        return "";
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }

    return result;
}
