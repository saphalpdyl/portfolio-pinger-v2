#include "process_extractor.h"


ProcessExtractor::ProcessExtractor(ILogger &logger, IConfigurationManager &configuration_manager,
    IProcessExtractorHelpers &process_extractor_helpers): _configuration_manager(configuration_manager), _process_extractor_helpers(process_extractor_helpers), _logger(logger) {
}

std::vector<ProcessTargetResult> ProcessExtractor::get_process_targets() const {
    const auto configuration = _configuration_manager.get_configuration();
    std::vector<ProcessTargetResult> results;

    for ( auto&[application_name, process_name] : configuration->process_targets ) {
        const auto is_running = _process_extractor_helpers.get_process_is_running(process_name);
        ProcessTargetResult result;
        result.application_name = application_name;
        result.process_name = process_name;
        result.is_running = is_running;

        results.push_back(result);
    }

    return results;
}
