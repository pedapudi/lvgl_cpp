#ifndef LVGL_CPP_WIDGETS_BAR_H_
#define LVGL_CPP_WIDGETS_BAR_H_

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export


#if LV_USE_BAR
/**
 * @file bar.h
 * @brief C++ Wrapper for LVGL Bar Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Bar widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {
class Subject;
class Observer;

/**
 * @brief Wrapper for lv_bar.
 */
class Bar : public Object {
 public:
  /**
   * @brief Create a new Bar.
   * @param parent Parent object.
   */
  Bar();
  explicit Bar(Object* parent);

  /**
   * @brief Wrap an existing lv_bar object.
   */
  explicit Bar(lv_obj_t* obj);

  void set_value(int32_t value, lv_anim_enable_t anim = LV_ANIM_ON);
  void set_start_value(int32_t value, lv_anim_enable_t anim = LV_ANIM_ON);
  void set_range(int32_t min, int32_t max);
  void set_min_value(int32_t min);
  void set_max_value(int32_t max);
  void set_mode(lv_bar_mode_t mode);
  void set_orientation(lv_bar_orientation_t orientation);

  int32_t get_value() const;
  int32_t get_start_value() const;
  int32_t get_min_value() const;
  int32_t get_max_value() const;
  lv_bar_mode_t get_mode() const;
  lv_bar_orientation_t get_orientation() const;
  bool is_symmetrical() const;

  /**
   * @brief Bind the bar's value to an integer subject.
   * @param subject The integer subject to bind.
   * @return An Observer object. Keep this object alive to maintain the binding.
   */
  [[nodiscard]] Observer bind_value(Subject& subject);
};

/**
 * @file bar.h
 * @brief C++ Wrapper for LVGL Bar Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Bar widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl


#endif // LV_USE_BAR
#endif  // LVGL_CPP_WIDGETS_BAR_H_
