#ifndef COMMANDER_H
#define COMMANDER_H
#include "service_manager.h"

class ICommander {
public:
    virtual ~ICommander() = default;
    virtual bool process_arguments(int argc, char* argv[]) const = 0;
};

class Commander final: public ICommander {
public:
    Commander(ILogger &logger, IServiceManager &service_manager)
        : _logger(logger),
          _service_manager(service_manager) {}

    bool process_arguments(int argc, char* argv[]) const override;

private:
    ILogger& _logger;
    IServiceManager& _service_manager;
};


#endif //COMMANDER_H
