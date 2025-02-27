#ifndef PROCESS_EXTRACTOR_HELPER_H
#define PROCESS_EXTRACTOR_HELPER_H

#include <string>

#include "logger.h"

class IProcessExtractorHelpers {
public:
    virtual ~IProcessExtractorHelpers() = default;

    virtual std::string exec(const char* cmd) = 0;
    virtual std::string get_service_status(const std::string& process_name) = 0;
};

class ProcessExtractorHelpers final: public IProcessExtractorHelpers {
public:
    explicit ProcessExtractorHelpers(ILogger &logger);

    std::string exec(const char *cmd) override;
    std::string get_service_status(const std::string& process_name) override;

private:
    ILogger& _logger;

};


#endif //PROCESS_EXTRACTOR_HELPER_H