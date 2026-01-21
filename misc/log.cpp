#include "log.h"

#include <cstdarg>
#include <cstdio>

namespace lvgl {

Log::Handler Log::handler_ = nullptr;

void Log::set_handler(Handler handler) {
  handler_ = std::move(handler);
  if (handler_) {
    lv_log_register_print_cb(log_proxy);
  } else {
    lv_log_register_print_cb(nullptr);
  }
}

void Log::clear_handler() {
  handler_ = nullptr;
  lv_log_register_print_cb(nullptr);
}

void Log::log_proxy(lv_log_level_t level, const char* buf) {
  if (handler_) {
    handler_(static_cast<LogLevel>(level), std::string_view(buf));
  }
}

void Log::log(LogLevel level, const char* format, ...) {
  if (static_cast<int>(level) < LV_LOG_LEVEL_TRACE) return;

  // We need to implement the formatting part because lv_log_add uses specific
  // internal logic or macros that might not be easily invoked directly with
  // va_list exposed cleanly. However, lv_log helper macros call _lv_log_add.
  // LVGL's logging macros are: LV_LOG_...(format, ...) -> _lv_log_add(level,
  // file, line, func, format, ...) Since we are wrapping the *interface*,
  // invoking LVGL's log system from here effectively means we are the "source".

  // Actually, LVGL's generic log function isn't easily exposed as a variadic
  // function that takes explicit level WITHOUT file/line info if we use
  // _lv_log_add. But strictly speaking, the user wanted a wrapper for
  // callbacks. If we want `Log::info("msg")` to go through LVGL and THEN back
  // to our handler, that works.

  // A simple way to inject into LVGL log stream:
  // But wait, LV_LOG implementation depends on LV_LOG_PRINTF or custom
  // callback. If we registered a callback, LVGL will call it.

  // To inject a log message into LVGL (so it comes back to us or goes to other
  // handlers): We can use the internal function `_lv_log_add`. It is usually
  // exposed in lv_log.h

  va_list args;
  va_start(args, format);
  // lv_log_add doesn't take va_list. It takes variadic directly.
  // We can use vhsnprintf to format locally and then pass string?
  // Or just call the callback directly?
  // Ideally we should route through LVGL so filters apply.

  // _lv_log_add declaration:
  // void _lv_log_add(lv_log_level_t level, const char * file, int line, const
  // char * func, const char * format, ...)

  // Since we can't easily forward va_list to ... function in C++,
  // and we don't want to duplicate formatting logic.

  // Proper C++ solution: Helper to call _lv_log_add is hard.
  // Alternative: Format string first.

  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);

  // _lv_log_add declaration:
  //   // Bypass lv_log_add to provide raw message to handler
  // This avoids double-formatting and allows structured logging
  if (handler_) {
    handler_(level, std::string_view(buf));
  }
}

void Log::trace(const char* format, ...) {
  if (static_cast<int>(LogLevel::Trace) < LV_LOG_LEVEL_TRACE) return;
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  if (handler_) handler_(LogLevel::Trace, std::string_view(buf));
}

void Log::info(const char* format, ...) {
  if (static_cast<int>(LogLevel::Info) < LV_LOG_LEVEL_INFO) return;
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  if (handler_) handler_(LogLevel::Info, std::string_view(buf));
}

void Log::warn(const char* format, ...) {
  if (static_cast<int>(LogLevel::Warn) < LV_LOG_LEVEL_WARN) return;
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  if (handler_) handler_(LogLevel::Warn, std::string_view(buf));
}

void Log::error(const char* format, ...) {
  // Error is usually always enabled, but check defines
#if LV_LOG_LEVEL <= LV_LOG_LEVEL_ERROR
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  if (handler_) handler_(LogLevel::Error, std::string_view(buf));
#endif
}

void Log::user(const char* format, ...) {
#if LV_LOG_LEVEL <= LV_LOG_LEVEL_USER
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  if (handler_) handler_(LogLevel::User, std::string_view(buf));
#endif
}

}  // namespace lvgl
