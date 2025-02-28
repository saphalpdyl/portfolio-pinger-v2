#ifndef PROCESS_EXTRACTOR_H
#define PROCESS_EXTRACTOR_H

#include <vector>

#include "configuration_manager.h"
#include "process_extractor_helper.h"
#include "process_target_result.h"

class IProcessExtractor {
public:
    virtual ~IProcessExtractor() = default;

    [[nodiscard]] virtual std::vector<ProcessTargetResult> get_process_targets() const = 0;
};

class ProcessExtractor final : public IProcessExtractor {
public:
    ProcessExtractor(
        ILogger& logger,
        IConfigurationManager& configuration_manager,
        IProcessExtractorHelpers& process_extractor_helpers
        );
    ~ProcessExtractor() override = default;

    [[nodiscard]] std::vector<ProcessTargetResult> get_process_targets() const override;

private:
    IConfigurationManager& _configuration_manager;
    IProcessExtractorHelpers& _process_extractor_helpers;
    ILogger& _logger;
};



#endif //PROCESS_EXTRACTOR_H
