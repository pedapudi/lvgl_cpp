#ifndef LVGL_CPP_MISC_LOG_H_
#define LVGL_CPP_MISC_LOG_H_

#include <functional>
#include <string_view>

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

/**
 * @brief Scoped enum for log levels.
 * Matches LVGL log levels.
 */
enum class LogLevel : int8_t {
  Trace = LV_LOG_LEVEL_TRACE,
  Info = LV_LOG_LEVEL_INFO,
  Warn = LV_LOG_LEVEL_WARN,
  Error = LV_LOG_LEVEL_ERROR,
  User = LV_LOG_LEVEL_USER,
  None = LV_LOG_LEVEL_NONE
};

/**
 * @brief Utility class for LVGL logging configuration.
 *
 * All methods are static since logging is a global operation in LVGL.
 * Wraps lv_log_register_print_cb() to allow C++ functors.
 */
class Log {
 public:
  /**
   * @brief Log handler callback type.
   * @param level The log severity level.
   * @param message The log message string.
   */
  using Handler = std::function<void(LogLevel level, std::string_view message)>;

  /**
   * @brief Set a custom log handler.
   *
   * @param handler The callback to receive log messages.
   *                Pass nullptr (or call clear_handler) to remove.
   *
   * @example
   * lvgl::Log::set_handler([](lvgl::LogLevel level, std::string_view msg) {
   *     std::cerr << "[LVGL] " << msg << std::endl;
   * });
   */
  static void set_handler(Handler handler);

  /**
   * @brief Clear the custom log handler.
   * Reverts to default LVGL logging (or no logging if LVGL default is none).
   */
  static void clear_handler();

  /**
   * @brief Log a message at the specified level.
   * @param level The severity level.
   * @param format The printf-style format string.
   */
  static void log(LogLevel level, const char* format, ...);

  /**
   * @brief Log a trace message.
   */
  static void trace(const char* format, ...);

  /**
   * @brief Log an info message.
   */
  static void info(const char* format, ...);

  /**
   * @brief Log a warning message.
   */
  static void warn(const char* format, ...);

  /**
   * @brief Log an error message.
   */
  static void error(const char* format, ...);

  /**
   * @brief Log a user message.
   */
  static void user(const char* format, ...);

 private:
  Log() = delete;

  static Handler handler_;
  static void log_proxy(lv_log_level_t level, const char* buf);
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_LOG_H_
