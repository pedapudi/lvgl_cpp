#include "log.h"

#include <cstdarg>
#include <cstdio>

namespace lvgl {

#if LV_USE_LOG

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

  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);

  // Manual dispatch for generic log since there is no generic LV_LOG macro
  // exposed openly that takes a variable level easily (lv_log_add is the
  // function but macro is preferred). We stick to direct dispatch here to avoid
  // the previous segfault issues with manual lv_log_add calls.
  if (handler_) {
    handler_(level, std::string_view(buf));
  }
}

void Log::trace(const char* format, ...) {
#if LV_LOG_LEVEL <= LV_LOG_LEVEL_TRACE
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  LV_LOG_TRACE("%s", buf);
#endif
}

void Log::info(const char* format, ...) {
#if LV_LOG_LEVEL <= LV_LOG_LEVEL_INFO
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  LV_LOG_INFO("%s", buf);
#endif
}

void Log::warn(const char* format, ...) {
#if LV_LOG_LEVEL <= LV_LOG_LEVEL_WARN
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  LV_LOG_WARN("%s", buf);
#endif
}

void Log::error(const char* format, ...) {
#if LV_LOG_LEVEL <= LV_LOG_LEVEL_ERROR
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  LV_LOG_ERROR("%s", buf);
#endif
}

void Log::user(const char* format, ...) {
#if LV_LOG_LEVEL <= LV_LOG_LEVEL_USER
  va_list args;
  va_start(args, format);
  char buf[256];
  vsnprintf(buf, sizeof(buf), format, args);
  va_end(args);
  LV_LOG_USER("%s", buf);
#endif
}

#endif  // LV_USE_LOG

}  // namespace lvgl
