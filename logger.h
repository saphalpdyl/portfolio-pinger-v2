#ifndef LOGGER_H
#define LOGGER_H

#include <syslog.h>
#include <iostream>
#include "constants.h"

enum class LoggerMode {
  CONSOLE,
  SYSLOG
};

// Interface for Logger, later used for mocking and testing
class ILogger {
public:
  virtual ~ILogger() = default;
  virtual void log(int log_level, const std::string& message) = 0;

protected:
  LoggerMode _mode = LoggerMode::SYSLOG;
  static std::string get_log_prefix(int log_level);
};

class Logger final : public ILogger {
public:
  explicit Logger(LoggerMode mode);
  ~Logger() override;

  void log( int log_level, const std::string& message) override;
};

#endif //LOGGER_H
