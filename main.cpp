#include <curlcpp/curl_easy.h>

#include "configuration_manager.h"

using curl::curl_easy;
using curl::curl_easy_exception;
using curl::curlcpp_traceback;

// TODOS:
// [x] Create a configuration loading system
// [] Handle network requests and errors
// [] Handle HMAC hashing
// [] Retrieve open application data from the system

int main() {
    Logger logger(LoggerMode::CONSOLE);
    logger.log(LOG_INFO, "Starting up...");

    // Dependency injection
    ConfigurationManager config_manager(logger);
    return 0;
}
