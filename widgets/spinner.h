#ifndef LVGL_CPP_WIDGETS_SPINNER_H_
#define LVGL_CPP_WIDGETS_SPINNER_H_

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
  explicit Spinner(Object* parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Spinner(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  void set_anim_params(uint32_t t, uint32_t angle);
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


#endif // LV_USE_SPINNER
#endif  // LVGL_CPP_WIDGETS_SPINNER_H_
