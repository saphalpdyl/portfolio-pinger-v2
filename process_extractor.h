#ifndef PROCESS_EXTRACTOR_H
#define PROCESS_EXTRACTOR_H

#include <vector>

#include "configuration_manager.h"
#include "process_extractor_helper.h"
#include "process_target_result.h"

class IProcessExtractor {
public:
    explicit IProcessExtractor(
        std::shared_ptr<ConfigurationManager> configuration_manager,
        std::shared_ptr<ProcessExtractorHelpers> process_extractor_helpers
    );

    virtual ~IProcessExtractor() = default;

    [[nodiscard]] virtual std::vector<ProcessTargetResult> get_process_targets() const = 0;
private:
    std::shared_ptr<ConfigurationManager> _configuration_manager;
    std::shared_ptr<ProcessExtractorHelpers> _process_extractor_helpers;
};

class ProcessExtractor final : public IProcessExtractor {
public:
    ProcessExtractor(
        std::shared_ptr<ConfigurationManager> configuration_manager,
        std::shared_ptr<ProcessExtractorHelpers> process_extractor_helpers
        );
    ~ProcessExtractor() override = default;

    [[nodiscard]] std::vector<ProcessTargetResult> get_process_targets() const override;
};



#endif //PROCESS_EXTRACTOR_H
