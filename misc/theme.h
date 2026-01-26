#ifndef LVGL_CPP_MISC_THEME_H_
#define LVGL_CPP_MISC_THEME_H_

#include "../core/object.h"
#include "../display/display.h"
#include "lvgl.h"

namespace lvgl {

class Theme {
 public:
  Theme() = default;
  explicit Theme(lv_theme_t* theme) : theme_(theme) {}

  /**
   * @brief Set the current theme for a display.
   * @param disp The display to apply the theme to (default: current default
   * display).
   */
  void set_current(Display* disp = nullptr);

  /**
   * @brief Apply this theme to an object.
   * @param obj The object.
   */
  void apply(Object& obj);

  /**
   * @brief Get the underlying theme pointer.
   */
  lv_theme_t* raw() const { return theme_; }

  /**
   * @brief Get the theme assigned to a display.
   * @param disp The display (default: current default display).
   */
  static Theme get_theme(Display* disp = nullptr);

 private:
  lv_theme_t* theme_ = nullptr;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_THEME_H_
