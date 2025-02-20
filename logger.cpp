#include "logger.h"

Logger::Logger(const LoggerMode mode) {
  _mode = mode;
  if ( mode == LoggerMode::SYSLOG ) {
    openlog(PINGER_CONFIG_SYSLOG_IDENT, LOG_PID | LOG_CONS, LOG_USER);
  }
}

Logger::~Logger() {
  if ( _mode == LoggerMode::SYSLOG ) {
    closelog();
  }
}

void Logger::log(const int log_level, const std::string &message) {
  if ( _mode == LoggerMode::SYSLOG ) {
    syslog(log_level, "%s", message.c_str());
  } else {
    const auto prefixed_message = get_log_prefix(log_level) + message;
    std::cout << prefixed_message << std::endl;
  }
}

std::string ILogger::get_log_prefix(const int log_level) {
  switch (log_level) {
    case LOG_EMERG:
      return "EMERGENCY: ";
    case LOG_ALERT:
      return "ALERT: ";
    case LOG_CRIT:
      return "CRITICAL: ";
    case LOG_ERR:
      return "ERROR: ";
    case LOG_WARNING:
      return "WARNING: ";
    case LOG_NOTICE:
      return "NOTICE: ";
    case LOG_INFO:
      return "INFO: ";
    case LOG_DEBUG:
      return "DEBUG: ";
    default:
      return "UNKNOWN: ";
  }
}