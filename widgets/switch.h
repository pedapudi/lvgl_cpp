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

  /**
   * @brief Switch orientations.
   */
  enum class Orientation : uint8_t {
    Auto = LV_SWITCH_ORIENTATION_AUTO,
    Horizontal = LV_SWITCH_ORIENTATION_HORIZONTAL,
    Vertical = LV_SWITCH_ORIENTATION_VERTICAL,
  };

  Switch& set_orientation(Orientation orientation);

  [[deprecated("Use set_orientation(Orientation) instead")]]
  Switch& set_orientation(lv_switch_orientation_t orientation);

  /**
   * @brief Register a callback for the ValueChanged event.
   * @param cb The callback function.
   */
  Switch& on_value_changed(std::function<void(lvgl::Event&)> cb);
  Orientation get_orientation() const;
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
