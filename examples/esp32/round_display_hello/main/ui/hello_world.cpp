#include "hello_world.h"

#include "esp_log.h"
#include "widgets/screen.h"

void HelloWorld::load(lvgl::Display& display, const std::string& text,
                      lvgl::Color bg_color, lvgl::Color text_color) {
  /*
   * Active Screen Management
   * ------------------------
   * In LVGL 9, we usually modify the current active screen.
   */
  auto screen = display.get_screen_active();
  lvgl::Screen scr(screen);
  scr.style().bg_color(bg_color);

  /*
   * Creating a Label Widget
   * -----------------------
   * We attach the label to the screen and configure it.
   * Circular scrolling is enabled if the text is too long for the width.
   */
  lvgl::Label label(&scr);
  label.set_long_mode(lvgl::Label::LongMode::ScrollCircular)
      .set_text(text)
      .set_width(display.get_horizontal_resolution() - 40)
      .center();

  label.style()
      .text_font(lvgl::Font(&lv_font_montserrat_20))
      .text_color(text_color);

  /*
   * Animation Example
   * -----------------
   * Animations in LVGL are set up by defining a target object, a range of
   * values, a duration, and a callback function to execute (e.g., set Y
   * coordinate).
   */
  lvgl::Animation(label)
      .set_values(-50, 0)  // Start off-screen and slide to center
      .set_duration(2000)
      .set_exec_cb([](lvgl::Object& obj, int32_t v) {
        // Use new typed callback (Issue #170)
        obj.set_y(v);
      })
      .set_path_cb(lvgl::Animation::Path::EaseOut())
      .set_completed_cb([]() { ESP_LOGI("UI", "Animation completed!"); })
      .start();

  /*
   * A Note on MEMORY MANAGEMENT:
   * ----------------------------
   * The 'label' object is a C++ wrapper. When it goes out of scope at the end
   * of this function, we don't want it to delete the underlying LVGL C-object
   * (which must stay alive to be displayed).
   *
   * .release() detaches the C++ wrapper from the C memory.
   */
  label.release();
}
