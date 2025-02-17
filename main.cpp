#include <iostream>
#include <curlcpp/curl_easy.h>

using curl::curl_easy;
using curl::curl_easy_exception;
using curl::curlcpp_traceback;

// TODOS:
// [] Create a configuration loading system
// [] Handle network requests and errors
// [] Handle HMAC hashing
// [] Retrieve open application data from the system

int main() {
    curl_easy easy;

    easy.add<CURLOPT_URL>("https://jsonplaceholder.typicode.com/todos/1");
    easy.add<CURLOPT_FOLLOWLOCATION>(1L);

    try {
        easy.perform();
    } catch ( curl_easy_exception &error ) {
        std::cerr << error.what() << std::endl;
    }

    return 0;
}