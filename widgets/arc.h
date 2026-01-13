#ifndef LVGL_CPP_WIDGETS_ARC_H_
#define LVGL_CPP_WIDGETS_ARC_H_

#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export

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
  explicit Arc(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Arc(lv_obj_t* obj);

  void set_start_angle(lv_value_precise_t start);
  void set_end_angle(lv_value_precise_t end);
  void set_angles(lv_value_precise_t start, lv_value_precise_t end);
  void set_bg_start_angle(lv_value_precise_t start);
  void set_bg_end_angle(lv_value_precise_t end);
  void set_bg_angles(lv_value_precise_t start, lv_value_precise_t end);
  void set_rotation(int32_t rotation);
  void set_mode(lv_arc_mode_t type);
  void set_value(int32_t value);
  void set_range(int32_t min, int32_t max);
  void set_min_value(int32_t min);
  void set_max_value(int32_t max);
  void set_change_rate(uint32_t rate);
  void set_knob_offset(int32_t offset);

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
} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_ARC_H_
