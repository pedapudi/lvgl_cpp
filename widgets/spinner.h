#ifndef LVGL_CPP_WIDGETS_SPINNER_H_
#define LVGL_CPP_WIDGETS_SPINNER_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
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

class Spinner : public Widget<Spinner> {
 public:
  Spinner();
  explicit Spinner(Object* parent, Ownership ownership = Ownership::Default);
  explicit Spinner(Object& parent);
  explicit Spinner(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a Spinner with specified animation parameters.
   * @param parent The parent object.
   * @param time Spin time in milliseconds.
   * @param arc_length Length of the arc in degrees.
   */
  Spinner(Object& parent, uint32_t time, uint32_t arc_length);

  Spinner& set_anim_params(uint32_t t, uint32_t angle);
};

}  // namespace lvgl

#endif  // LV_USE_SPINNER
#endif  // LVGL_CPP_WIDGETS_SPINNER_H_
