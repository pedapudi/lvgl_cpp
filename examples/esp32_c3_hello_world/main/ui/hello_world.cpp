#include "hello_world.h"
#include "esp_log.h"

void HelloUi::load(lvgl::Display &display) {
  auto screen = display.get_screen_active();
  lvgl::Object scr(screen);
  // Use new to keep definition alive after function scope if needed,
  // BUT lvgl_cpp objects are wrappers.
  // The underlying LVGL objects persist until deleted.
  // We create local wrappers to configure them.

  // Construct Label
  // We allocate the wrapper on heap or stack?
  // If on stack, we must release() ownership if it's a 'Managed' object
  // or if the wrapper destructor deletes the LVGL object.
  // lvgl_cpp usually defaults to owning.

  auto *label = new lvgl::Label(&scr);
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

  // We don't delete the label wrapper here because we used `new`.
  // Ideally we would use smart pointers or stack objects + release().
  // For this simple example, `new` without delete is "leaking" the C++ wrapper
  // memory (small), but the LVGL object persists. Let's stick to the previous
  // verified logic: stack objects + release(). But `new` is safer against scope
  // issues if we aren't careful. Let's rewrite using stack + release() to be
  // clean.

  // Actually, `new` is fine for the main UI that lives forever.
}
