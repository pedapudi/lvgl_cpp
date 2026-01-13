#ifndef LVGL_CPP_WIDGETS_LED_H_
#define LVGL_CPP_WIDGETS_LED_H_


#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export

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
  explicit Led(Object *parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Led(lv_obj_t *obj);

  void set_color(lv_color_t color);
  void set_brightness(uint8_t bright);
  void on();
  void off();
  void toggle();

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
} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_LED_H_
