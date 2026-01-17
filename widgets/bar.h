#ifndef LVGL_CPP_WIDGETS_BAR_H_
#define LVGL_CPP_WIDGETS_BAR_H_

#include <cstdint>

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
  explicit Bar(Object& parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Wrap an existing lv_bar object.
   */
  explicit Bar(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Set the current value.
   * @param value The value to set.
   * @param anim Enable animation.
   */
  Bar& set_value(int32_t value, lv_anim_enable_t anim = LV_ANIM_ON);

  /**
   * @brief Set the start value (for range bars).
   * @param value The start value.
   * @param anim Enable animation.
   */
  Bar& set_start_value(int32_t value, lv_anim_enable_t anim = LV_ANIM_ON);

  /**
   * @brief Set the range of the bar.
   * @param min Minimum value.
   * @param max Maximum value.
   */
  Bar& set_range(int32_t min, int32_t max);

  /**
   * @brief Set the minimum value.
   * @param min Minimum value.
   */
  Bar& set_min_value(int32_t min);

  /**
   * @brief Set the maximum value.
   * @param max Maximum value.
   */
  Bar& set_max_value(int32_t max);

  /**
   * @brief Set the mode of the bar.
   * @param mode Bar mode (`LV_BAR_MODE_NORMAL` or `LV_BAR_MODE_RANGE`).
   */
  Bar& set_mode(lv_bar_mode_t mode);

  /**
   * @brief Set the orientation of the bar.
   * @param orientation Orientation (`LV_BAR_ORIENTATION_HORIZONTAL`, etc.).
   */
  Bar& set_orientation(lv_bar_orientation_t orientation);

  // Fluent API shadows
  Bar& set_width(int32_t width);
  Bar& set_height(int32_t height);
  Bar& set_size(int32_t width, int32_t height);
  Bar& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Bar& add_state(lv_state_t state);
  Bar& remove_state(lv_state_t state);
  Bar& add_flag(lv_obj_flag_t flag);
  Bar& remove_flag(lv_obj_flag_t flag);

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

}  // namespace lvgl

#endif  // LV_USE_BAR
#endif  // LVGL_CPP_WIDGETS_BAR_H_
