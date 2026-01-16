#include "hello_world.h"
#include "esp_log.h"

#include <memory> 

void HelloUi::load(lvgl::Display &display) {
  auto screen = display.get_screen_active();
  lvgl::Object scr(screen);

  // Use unique_ptr to manage the wrapper lifetime during configuration.
  auto label = std::make_unique<lvgl::Label>(&scr);
  label->set_long_mode(LV_LABEL_LONG_SCROLL_CIRCULAR);
  label->set_text("Hello Espressif, Hello LVGL.");
  label->set_width(display.get_horizontal_resolution());
  label->align(LV_ALIGN_BOTTOM_MID, 0, 0);

  // Animation
  lvgl::Animation(*label)
      .set_values(-50, 0)
      .set_duration(2000)
      .set_exec_cb(lvgl::Animation::Exec::Y())
      .set_path_cb(lvgl::Animation::Path::EaseOut())
      .set_completed_cb([]() { ESP_LOGI("UI", "Animation completed!"); })
      .start();

  // Release ownership so the C++ wrapper's destructor (when this function ends)
  // does not destroy the underlying LVGL object.
  label->release();
}
