#include "process_extractor.h"

ProcessExtractor::ProcessExtractor(std::shared_ptr<ConfigurationManager> configuration_manager,
    std::shared_ptr<ProcessExtractorHelpers> process_extractor_helpers) {
}

std::vector<ProcessTargetResult> ProcessExtractor::get_process_targets() const {
    return {};
}
