#ifndef LVGL_CPP_WIDGETS_SPINBOX_H_
#define LVGL_CPP_WIDGETS_SPINBOX_H_

#include <cstdint>

#include "lvgl.h"      // IWYU pragma: export
#include "textarea.h"  // IWYU pragma: export


#if LV_USE_SPINBOX
/**
 * @file spinbox.h
 * @brief C++ Wrapper for LVGL Spinbox Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Spinbox widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Spinbox : public Textarea {
 public:
  /**
   * @brief Create a Spinbox on the active screen.
   */
  Spinbox();
  /**
   * @brief Create a Spinbox with a parent.
   * @param parent The parent object.
   */
  explicit Spinbox(Object* parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Spinbox(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  void set_value(int32_t v);
  void set_rollover(bool rollover);
  void set_digit_format(uint32_t digit_count, uint32_t sep_pos);
  void set_digit_count(uint32_t digit_count);
  void set_dec_point_pos(uint32_t dec_point_pos);
  void set_step(uint32_t step);
  void set_range(int32_t min_value, int32_t max_value);
  void set_min_value(int32_t min_value);
  void set_max_value(int32_t max_value);
  // hide set_cursor_pos from Textarea if signature mismatches or just overload
  void set_cursor_pos(uint32_t pos);
  void set_digit_step_direction(lv_dir_t direction);

  bool get_rollover();
  int32_t get_value();
  int32_t get_step();

  void step_next();
  void step_prev();
  void increment();
  void decrement();
};

/**
 * @file spinbox.h
 * @brief C++ Wrapper for LVGL Spinbox Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Spinbox widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl


#endif // LV_USE_SPINBOX
#endif  // LVGL_CPP_WIDGETS_SPINBOX_H_
