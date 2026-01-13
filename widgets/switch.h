#ifndef LVGL_CPP_WIDGETS_SWITCH_H_
#define LVGL_CPP_WIDGETS_SWITCH_H_


#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export

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
  explicit Switch(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Switch(lv_obj_t* obj);

  void set_orientation(lv_switch_orientation_t orientation);
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
} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_SWITCH_H_
