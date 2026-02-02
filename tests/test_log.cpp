#include <iostream>
#include <string>
#include <vector>

#include "../misc/log.h"
#include "../lvgl_cpp.h"

static std::vector<std::string> log_messages;
static std::vector<lvgl::LogLevel> log_levels;

void test_log_handler() {
  std::cout << "Testing Log Handler..." << std::endl;
  log_messages.clear();
  log_levels.clear();

  lvgl::Log::set_handler([](lvgl::LogLevel level, std::string_view msg) {
    log_levels.push_back(level);
    log_messages.push_back(std::string(msg));
  });

  // Test explicit log calls (these route through LVGL)
  // Note: If LV_LOG_LEVEL is set high in lv_conf.h, lower priority logs will be
  // dropped before reaching our callback. We assume standard config here or we
  // test high priority logs.

  lvgl::Log::user("User message %d", 1);
  lvgl::Log::error("Error message");

  if (log_messages.size() >= 2) {
    std::cout << "PASS: Received " << log_messages.size() << " messages."
              << std::endl;
    // We expect at least "User message 1" and "Error message"
    bool found_user = false;
    bool found_error = false;
    for (const auto& msg : log_messages) {
      if (msg.find("User message 1") != std::string::npos) found_user = true;
      if (msg.find("Error message") != std::string::npos) found_error = true;
    }

    if (found_user && found_error) {
      std::cout << "PASS: Content verified." << std::endl;
    } else {
      std::cerr << "FAIL: Content mismatch." << std::endl;
      for (const auto& msg : log_messages)
        std::cerr << "  Received: " << msg << std::endl;
      exit(1);
    }
  } else {
// It's possible LV_LOG_LEVEL is disabling some logs.
// But USER and ERROR are usually enabled.
// If messages are empty, it might be that LV_USE_LOG is 0.
#if LV_USE_LOG
    if (log_messages.empty()) {
      std::cerr << "FAIL: No messages received. Is logging enabled?"
                << std::endl;
      exit(1);
    }
#else
    std::cout << "SKIP: LV_USE_LOG is 0." << std::endl;
#endif
  }
}

void test_clear_handler() {
  std::cout << "Testing Clear Handler..." << std::endl;
  log_messages.clear();

  lvgl::Log::clear_handler();

  lvgl::Log::error("This should not be captured");

  if (log_messages.empty()) {
    std::cout << "PASS: No messages captured after clear." << std::endl;
  } else {
    std::cerr << "FAIL: Captured message after clear." << std::endl;
    exit(1);
  }
}

int main() {
  lv_init();

  // Ensure logging is enabled for valid testing if possible?
  // We can't change compile-time LV_USE_LOG.

  test_log_handler();
  test_clear_handler();

  std::cout << "\nAll Log tests passed!" << std::endl;
  return 0;
}
