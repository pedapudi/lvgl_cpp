#ifndef LVGL_CPP_WIDGETS_LED_H_
#define LVGL_CPP_WIDGETS_LED_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
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

class Led : public Object {
 public:
  /**
   * @brief Create a Led on the active screen.
   */
  Led();
  /**
   * @brief Create a Led with a parent.
   * @param parent The parent object.
   */
  explicit Led(Object* parent, Ownership ownership = Ownership::Default);
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

  // Fluent API shadows
  Led& set_width(int32_t width);
  Led& set_height(int32_t height);
  Led& set_size(int32_t width, int32_t height);
  Led& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Led& add_state(lv_state_t state);
  Led& remove_state(lv_state_t state);
  Led& add_flag(lv_obj_flag_t flag);
  Led& remove_flag(lv_obj_flag_t flag);

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
