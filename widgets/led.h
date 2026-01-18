#ifndef LVGL_CPP_WIDGETS_LED_H_
#define LVGL_CPP_WIDGETS_LED_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_LED
/**
 * @file led.h
 * @brief C++ Wrapper for LVGL Led Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Led widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Led : public Widget<Led> {
 public:
  /**
   * @brief Create a Led on the active screen.
   */
  Led();
  /**
   * @brief Create a Led with a parent.
   * @param parent The parent object.
   */
  explicit Led(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Led(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Led& set_color(lv_color_t color);
  Led& set_brightness(uint8_t bright);
  Led& on();
  Led& off();
  Led& toggle();

  uint8_t get_brightness();
};

/**
 * @file led.h
 * @brief C++ Wrapper for LVGL Led Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Led widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl

#endif  // LV_USE_LED
#endif  // LVGL_CPP_WIDGETS_LED_H_
