#ifndef LVGL_CPP_WIDGETS_SWITCH_H_
#define LVGL_CPP_WIDGETS_SWITCH_H_

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_SWITCH
/**
 * @file switch.h
 * @brief C++ Wrapper for LVGL Switch Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Switch widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Switch : public Widget<Switch> {
 public:
  using Widget::Widget;

  /**
   * @brief Create a Switch on the active screen.
   */
  Switch();
  explicit Switch(Object* parent, Ownership ownership = Ownership::Default);
  explicit Switch(Object& parent);
  explicit Switch(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Switch& set_orientation(lv_switch_orientation_t orientation);
  lv_switch_orientation_t get_orientation() const;
};

/**
 * @file switch.h
 * @brief C++ Wrapper for LVGL Switch Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Switch widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl

#endif  // LV_USE_SWITCH
#endif  // LVGL_CPP_WIDGETS_SWITCH_H_
