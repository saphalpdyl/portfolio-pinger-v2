#ifndef COMMAND_EXECUTOR_H
#define COMMAND_EXECUTOR_H

#include <string>

#include "logger.h"

class ICommandExecutor {
public:
    virtual ~ICommandExecutor() = default;
    virtual std::string exec(const char* cmd) = 0;
};

class CommandExecutor final: public ICommandExecutor {
public:
    explicit CommandExecutor(ILogger &logger): _logger(logger) {}
    std::string exec(const char *cmd) override;
private:
    ILogger& _logger;
};


#endif //COMMAND_EXECUTOR_H
