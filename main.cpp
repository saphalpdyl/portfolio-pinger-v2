#include <curlcpp/curl_easy.h>

#include "configuration_manager.h"

using curl::curl_easy;
using curl::curl_easy_exception;
using curl::curlcpp_traceback;

// TODOS:
// [] Create a configuration loading system
// [] Handle network requests and errors
// [] Handle HMAC hashing
// [] Retrieve open application data from the system

int test_main();

int main() {

    return 0;
}

int test_main() {
    std::string test_configuration_directory = std::string(__FILE__).substr(0, std::string(__FILE__).rfind("/"));

    const ConfigurationManager configuration_manager("test.config.json", test_configuration_directory);

    if (auto result = configuration_manager.load_configuration(); result != PingerResult::OK ) {
        return 1;
    }


    return 0;
}