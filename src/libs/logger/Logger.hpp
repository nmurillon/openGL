#pragma once

#include <chrono>
#include <format>
#include <iostream>
#include <string>

class Logger {
public:
  enum class LogLevel { ERROR, WARNING, INFO, DEBUG, TRACE };

  static void setLogLevel(LogLevel level) { s_logLevel = level; }

  template <typename... Ts>
  static void logError(std::format_string<Ts...> format, Ts &&...args) {
    log(LogLevel::ERROR, format, args...);
  }

  template <typename... Ts>
  static void logWarning(std::format_string<Ts...> format, Ts &&...args) {
    log(LogLevel::WARNING, format, args...);
  }

  template <typename... Ts>
  static void logInfo(std::format_string<Ts...> format, Ts &&...args) {
    log(LogLevel::INFO, format, args...);
  }

  template <typename... Ts>
  static void logDebug(std::format_string<Ts...> format, Ts &&...args) {
    log(LogLevel::DEBUG, format, args...);
  }

  template <typename... Ts>
  static void logTrace(std::format_string<Ts...> format, Ts &&...args) {
    log(LogLevel::TRACE, format, args...);
  }

private:
  template <typename... Ts>
  static void log(LogLevel level, std::format_string<Ts...> format,
                  Ts &&...args) {
    if (s_logLevel < level) {
      return;
    }

    const auto now = std::chrono::system_clock::now();
    std::string s;
    auto it = std::back_inserter(s);
    std::format_to(it, std::forward<std::format_string<Ts...>>(format),
                   std::forward<Ts>(args)...);
    std::cout << std::format("[{}]{} ", now, logLevelToStr(level)) << s
              << std::endl;
  }

  static std::string logLevelToStr(LogLevel level) {
    switch (level) {
    case LogLevel::ERROR:
      return "[ERROR]";
      break;
    case LogLevel::WARNING:
      return "[WARNING]";
      break;
    case LogLevel::INFO:
      return "[INFO]";
      break;
    case LogLevel::DEBUG:
      return "[DEBUG]";
      break;
    case LogLevel::TRACE:
      return "[TRACE]";
      break;
    default:
      return "[UNEXISTING_LOG_LEVEL]";
      break;
    }
  }

private:
  inline static LogLevel s_logLevel{LogLevel::DEBUG};
};