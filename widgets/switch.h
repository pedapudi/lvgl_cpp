#ifndef LVGL_CPP_WIDGETS_SWITCH_H_
#define LVGL_CPP_WIDGETS_SWITCH_H_

#include "../core/object.h"  // IWYU pragma: export
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

class Switch : public Object {
 public:
  /**
   * @brief Create a Switch on the active screen.
   */
  Switch();
  /**
   * @brief Create a Switch with a parent.
   * @param parent The parent object.
   */
  explicit Switch(Object* parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Switch(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Switch& set_orientation(lv_switch_orientation_t orientation);
  lv_switch_orientation_t get_orientation() const;

  // Fluent API shadows
  Switch& set_width(int32_t width);
  Switch& set_height(int32_t height);
  Switch& set_size(int32_t width, int32_t height);
  Switch& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Switch& add_state(lv_state_t state);
  Switch& remove_state(lv_state_t state);
  Switch& add_flag(lv_obj_flag_t flag);
  Switch& remove_flag(lv_obj_flag_t flag);
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
