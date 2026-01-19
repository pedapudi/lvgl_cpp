#ifndef LVGL_CPP_WIDGETS_SPINBOX_H_
#define LVGL_CPP_WIDGETS_SPINBOX_H_

#include <cstdint>

#include "../../misc/enums.h"
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
  Spinbox();
  explicit Spinbox(Object* parent, Ownership ownership = Ownership::Default);
  explicit Spinbox(Object& parent);
  explicit Spinbox(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Spinbox& set_value(int32_t v);
  Spinbox& set_rollover(bool rollover);
  Spinbox& set_digit_format(uint32_t digit_count, uint32_t sep_pos);
  Spinbox& set_digit_count(uint32_t digit_count);
  Spinbox& set_dec_point_pos(uint32_t dec_point_pos);
  Spinbox& set_step(uint32_t step);
  Spinbox& set_range(int32_t min_value, int32_t max_value);
  Spinbox& set_min_value(int32_t min_value);
  Spinbox& set_max_value(int32_t max_value);
  Spinbox& set_cursor_pos(uint32_t pos);
  Spinbox& set_digit_step_direction(lv_dir_t direction);

  bool get_rollover();
  int32_t get_value();
  int32_t get_step();

  Spinbox& step_next();
  Spinbox& step_prev();
  Spinbox& increment();
  Spinbox& decrement();

  // Fluent API shadows
  Spinbox& set_width(int32_t width);
  Spinbox& set_height(int32_t height);
  Spinbox& set_size(int32_t width, int32_t height);
  Spinbox& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Spinbox& add_state(State state);
  Spinbox& remove_state(State state);
  Spinbox& add_flag(lv_obj_flag_t flag);
  Spinbox& remove_flag(lv_obj_flag_t flag);
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

#endif  // LV_USE_SPINBOX
#endif  // LVGL_CPP_WIDGETS_SPINBOX_H_
