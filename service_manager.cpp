#include "service_manager.h"

#include <fstream>
#include <thread>

#include "network_interface.h"
#include "process_extractor.h"
#include "process_extractor_helper.h"
#include "webhook_server.h"


bool ServiceManager::install_service() {
    _logger.log(LOG_INFO, "Installing service...");

    const std::string service_path = "/etc/systemd/system/" + SERVICE_NAME + ".service";
    const std::string exec_path = get_executable_path();

    const std::string service_content =
            "[Unit]\n"
            "Description=Process Monitor Service\n"
            "After=network.target\n\n"
            "[Service]\n"
            "Type=simple\n"
            "ExecStart=" + exec_path + " run\n"
            "Restart=on-failure\n"
            "RestartSec=5s\n\n"
            "[Install]\n"
            "WantedBy=multi-user.target\n";

    if (!write_file(service_path, service_content)) {
        _logger.log(LOG_ERR, "Failed to write service file.");
        return false;
    }

    std::vector<std::string> commands = {
    "systemctl daemon-reload",
        "systemctl enable " + SERVICE_NAME + ".service",
        "systemctl start " + SERVICE_NAME + ".service"
    };

    for ( const auto& cmd: commands ) {
        if ( !_command_executor.exec(cmd.c_str()).data() ) {
            _logger.log(LOG_ERR, "Failed to execute command: " + cmd);
            return false;
        }
    }

    _logger.log(LOG_INFO, "Service installed successfully.");
    return true;
}

bool ServiceManager::uninstall_service() const {
    _logger.log(LOG_INFO, "Uninstalling service...");

    const std::vector<std::string> commands = {
        "systemctl stop " + SERVICE_NAME + ".service",
        "systemctl disable " + SERVICE_NAME + ".service",
    };

    for ( const auto& cmd: commands ) {
        _command_executor.exec(cmd.c_str());
    }

    const std::string service_path = "/etc/systemd/system/" + SERVICE_NAME + ".service";
    if ( std::remove(service_path.c_str()) != 0 ) {
        _logger.log(LOG_WARNING, "Failed to remove service file.");
    }

    _command_executor.exec("systemctl daemon-reload");

    _logger.log(LOG_INFO, "Service uninstalled successfully.");
    return true;
}

bool ServiceManager::run_service() const {
    Logger logger(LoggerMode::SYSLOG);
    logger.log(LOG_INFO, "Running service...");

    _config_manager.load_file();
    _config_manager.deserialize_configuration();
    _config_manager.parse_configuration();

    if ( _config_manager.get_configuration() == nullptr ) {
        logger.log(LOG_ERR, "Couldn't load configuration.");
        return false;
    }

    ProcessExtractorHelpers process_extractor_helpers(logger, _command_executor);
    const ProcessExtractor process_extractor(logger, _config_manager, process_extractor_helpers);

    NetworkInterface network_interface;
    WebhookServer webhook_server(logger, network_interface, _config_manager);

    while (true) {
        try {
            auto results = process_extractor.get_process_targets();
            webhook_server.send_process_results(results);

            std::this_thread::sleep_for(std::chrono::milliseconds(_config_manager.get_configuration()->ping_interval));
        } catch ( const std::exception& e ) {
            logger.log(LOG_ERR, "Error in service loop: " + std::string(e.what()));
            std::this_thread::sleep_for(std::chrono::seconds(5));
        }
    }
}

std::string ServiceManager::get_executable_path() {
    char buff[PATH_MAX];
    if (const ssize_t len = readlink("/proc/self/exe", buff, sizeof(buff) - 1); len != -1) {
        buff[len] = '\0';
        return buff;
    }
    return "/usr/local/bin/" + SERVICE_NAME; // Fallback
}

bool ServiceManager::write_file(const std::string &path, const std::string &content) {
    std::ofstream file(path);
    if (!file.is_open()) {
        return false;
    }
    file << content;
    return file.good();
}
