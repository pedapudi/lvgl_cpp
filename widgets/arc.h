#ifndef LVGL_CPP_WIDGETS_ARC_H_
#define LVGL_CPP_WIDGETS_ARC_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_ARC
/**
 * @file arc.h
 * @brief C++ Wrapper for LVGL Arc Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Arc widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {
class Subject;
class Observer;

class Arc : public Object {
 public:
  /**
   * @brief Create a Arc on the active screen.
   */
  Arc();
  /**
   * @brief Create a Arc with a parent.
   * @param parent The parent object.
   */
  explicit Arc(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Arc(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Arc& set_start_angle(lv_value_precise_t start);
  Arc& set_end_angle(lv_value_precise_t end);
  Arc& set_angles(lv_value_precise_t start, lv_value_precise_t end);
  Arc& set_bg_start_angle(lv_value_precise_t start);
  Arc& set_bg_end_angle(lv_value_precise_t end);
  Arc& set_bg_angles(lv_value_precise_t start, lv_value_precise_t end);

  /**
   * @brief Set rotation (offset to 0 degree).
   * @param rotation Angle in degrees.
   */
  Arc& set_rotation(int32_t rotation);

  /**
   * @brief Set the arc mode.
   * @param type Mode (e.g., `LV_ARC_MODE_NORMAL`).
   */
  Arc& set_mode(lv_arc_mode_t type);

  /**
   * @brief Set the current value.
   * @param value The value.
   */
  Arc& set_value(int32_t value);

  /**
   * @brief Set the range.
   * @param min Minimum value.
   * @param max Maximum value.
   */
  Arc& set_range(int32_t min, int32_t max);

  Arc& set_min_value(int32_t min);
  Arc& set_max_value(int32_t max);

  /**
   * @brief Set the change rate (smoothing).
   * @param rate Rate in degrees/step.
   */
  Arc& set_change_rate(uint32_t rate);

  /**
   * @brief Set the knob offset.
   * @param offset Offset in pixels.
   */
  Arc& set_knob_offset(int32_t offset);

  // Fluent API shadows
  Arc& set_width(int32_t width);
  Arc& set_height(int32_t height);
  Arc& set_size(int32_t width, int32_t height);
  Arc& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Arc& add_state(lv_state_t state);
  Arc& remove_state(lv_state_t state);
  Arc& add_flag(lv_obj_flag_t flag);
  Arc& remove_flag(lv_obj_flag_t flag);

  lv_value_precise_t get_angle_start();
  lv_value_precise_t get_angle_end();
  lv_value_precise_t get_bg_angle_start();
  lv_value_precise_t get_bg_angle_end();
  int32_t get_value() const;
  int32_t get_min_value() const;
  int32_t get_max_value() const;
  lv_arc_mode_t get_mode() const;
  int32_t get_rotation() const;
  int32_t get_knob_offset() const;

  /**
   * @brief Bind the arc's value to an integer subject.
   * @param subject The integer subject to bind.
   * @return An Observer object. Keep this object alive to maintain the binding.
   */
  [[nodiscard]] Observer bind_value(Subject& subject);
};

}  // namespace lvgl

#endif  // LV_USE_ARC
#endif  // LVGL_CPP_WIDGETS_ARC_H_
