#include "hello_world.h"

#include "esp_log.h"

void HelloWorld::load(lvgl::Display& display, const std::string& text) {
  auto screen = display.get_screen_active();
  lvgl::Object scr(screen);

  // Construct Label on stack and release ownership so it persists
  lvgl::Label label(&scr);
  label.set_long_mode(lvgl::Label::LongMode::ScrollCircular);
  label.set_text(text);
  label.set_width(display.get_horizontal_resolution());
  label.align(lvgl::Align::BottomMid, 0, 0);

  // Animation
  lvgl::Animation(label)
      .set_values(-50, 0)
      .set_duration(2000)
      .set_exec_cb(lvgl::Animation::Exec::Y())
      .set_path_cb(lvgl::Animation::Path::EaseOut())
      .set_completed_cb([]() { ESP_LOGI("UI", "Animation completed!"); })
      .start();

  // Release ownership so the underlying LVGL object is NOT deleted
  // when the C++ wrapper goes out of scope.
  label.release();
}
