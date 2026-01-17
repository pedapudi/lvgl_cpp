#ifndef LVGL_CPP_WIDGETS_SPINNER_H_
#define LVGL_CPP_WIDGETS_SPINNER_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_SPINNER
/**
 * @file spinner.h
 * @brief C++ Wrapper for LVGL Spinner Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Spinner widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Spinner : public Object {
 public:
  /**
   * @brief Create a Spinner on the active screen.
   */
  Spinner();
  /**
   * @brief Create a Spinner with a parent.
   * @param parent The parent object.
   */
  explicit Spinner(Object& parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a Spinner with specified animation parameters.
   * @param parent The parent object.
   * @param time Spin time in milliseconds.
   * @param arc_length Length of the arc in degrees.
   */
  Spinner(Object& parent, uint32_t time, uint32_t arc_length);

  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Spinner(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Spinner& set_anim_params(uint32_t t, uint32_t angle);

  // Fluent API shadows
  Spinner& set_width(int32_t width);
  Spinner& set_height(int32_t height);
  Spinner& set_size(int32_t width, int32_t height);
  Spinner& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Spinner& add_state(lv_state_t state);
  Spinner& remove_state(lv_state_t state);
  Spinner& add_flag(lv_obj_flag_t flag);
  Spinner& remove_flag(lv_obj_flag_t flag);
};

/**
 * @file spinner.h
 * @brief C++ Wrapper for LVGL Spinner Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Spinner widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl

#endif  // LV_USE_SPINNER
#endif  // LVGL_CPP_WIDGETS_SPINNER_H_
