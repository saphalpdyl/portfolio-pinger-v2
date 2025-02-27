#ifndef PROCESS_EXTRACTOR_HELPER_H
#define PROCESS_EXTRACTOR_HELPER_H

#include <string>

#include "command_executor.h"
#include "logger.h"
#include "process_target_result.h"

class IProcessExtractorHelpers {
public:
    virtual ~IProcessExtractorHelpers() = default;

    virtual bool get_process_is_running(const std::string& process_name) = 0;
};

class ProcessExtractorHelpers final: public IProcessExtractorHelpers {
public:
    explicit ProcessExtractorHelpers(ILogger &logger, ICommandExecutor& command_executor);

    bool get_process_is_running(const std::string& process_name) override;

private:
    ILogger& _logger;
    ICommandExecutor& _command_executor;
};


#endif //PROCESS_EXTRACTOR_HELPER_H