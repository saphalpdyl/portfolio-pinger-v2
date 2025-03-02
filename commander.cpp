#include "commander.h"

bool Commander::process_arguments(int argc, char *argv[]) const {
    if ( argc < 2 ) {
        _logger.log(LOG_ERR, "No command provided");
        return false;
    }

    const std::string command = argv[1];

    if ( command == "install" ) {
        return _service_manager.install_service();
    }
    if ( command == "uninstall" ) {
        return _service_manager.uninstall_service();
    }
    if ( command == "run" ) {
        return _service_manager.run_service();
    }

    _logger.log(LOG_ERR, "Unknown command: " + command);
    return false;
}
