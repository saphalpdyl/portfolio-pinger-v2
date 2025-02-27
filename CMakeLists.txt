cmake_minimum_required(VERSION 3.30)
project(portfolio_pinger)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)

# Add executables
add_executable(portfolio_pinger main.cpp
        logger.h
        logger.cpp
        configuration_manager.cpp
        configuration_manager.h
        constants.h
        errors.h
        configuration.h
        process_target.h
        process_target_result.h
        process_extractor.cpp
        process_extractor.h
)

add_executable(
        GoogleTests_run
        main_test.cpp
        configuration_manager_test.cpp
        configuration_manager.h
        configuration_manager.cpp
        process_extractor.cpp
        process_extractor.h
        process_extractor_helper.h
        process_extractor_helper.cpp
        process_extractor_helper_test.cpp
        command_executor.cpp
        command_executor.h
)
target_compile_definitions(GoogleTests_run PRIVATE TESTING)

# Link libraries (vcpkg will handle finding these)
find_package(nlohmann_json REQUIRED)
find_package(GTest REQUIRED)
find_package(curlcpp REQUIRED)

target_link_libraries(portfolio_pinger PRIVATE curlcpp::curlcpp nlohmann_json::nlohmann_json)
target_link_libraries(GoogleTests_run PRIVATE GTest::GTest GTest::gtest_main GTest::gmock_main)