cmake_minimum_required(VERSION 3.30)
project(portfolio_pinger)

set(CMAKE_CXX_STANDARD 20)

add_executable(portfolio_pinger main.cpp
        configuration_manager.cpp
        configuration_manager.h
        constants.h
        errors.h
        configuration.h
        process_target.h
        process_target_result.h
)

add_executable(
        GoogleTests_run
        configuration_manager_test.cpp
        configuration_manager.h
        configuration_manager.cpp
)
target_compile_definitions(GoogleTests_run PRIVATE TESTING)

find_package(curlcpp CONFIG REQUIRED)
target_link_libraries(portfolio_pinger PRIVATE curlcpp::curlcpp)

find_package(nlohmann_json CONFIG REQUIRED)
target_link_libraries(portfolio_pinger PRIVATE nlohmann_json::nlohmann_json)

find_package(GTest CONFIG REQUIRED)
target_link_libraries(GoogleTests_run PRIVATE GTest::gtest_main)