#ifndef LVGL_CPP_WIDGETS_SCALE_H_
#define LVGL_CPP_WIDGETS_SCALE_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_SCALE
/**
 * @file scale.h
 * @brief C++ Wrapper for LVGL Scale Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Scale widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Scale : public Object {
 public:
  /**
   * @brief Create a Scale on the active screen.
   */
  Scale();
  /**
   * @brief Create a Scale with a parent.
   * @param parent The parent object.
   */
  explicit Scale(Object* parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Scale(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Scale& set_mode(lv_scale_mode_t mode);
  Scale& set_total_tick_count(uint32_t total_tick_count);
  Scale& set_major_tick_every(uint32_t major_tick_every);
  Scale& set_label_show(bool show_label);
  Scale& set_range(int32_t min, int32_t max);
  Scale& set_angle_range(uint32_t angle_range);
  Scale& set_rotation(int32_t rotation);
  Scale& set_line_needle_value(lv_obj_t* needle_line, int32_t needle_length,
                               int32_t value);
  Scale& set_image_needle_value(lv_obj_t* needle_img, int32_t value);
  Scale& set_text_src(const char* txt_src[]);
  Scale& set_post_draw(bool en);
  Scale& set_draw_ticks_on_top(bool en);

  // Fluent API shadows
  Scale& set_width(int32_t width);
  Scale& set_height(int32_t height);
  Scale& set_size(int32_t width, int32_t height);
  Scale& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Scale& add_state(lv_state_t state);
  Scale& remove_state(lv_state_t state);
  Scale& add_flag(lv_obj_flag_t flag);
  Scale& remove_flag(lv_obj_flag_t flag);

  lv_scale_mode_t get_mode();
  int32_t get_total_tick_count();
  int32_t get_major_tick_every();
  bool get_label_show();
  uint32_t get_angle_range();
  int32_t get_range_min_value();
  int32_t get_range_max_value();
};

/**
 * @file scale.h
 * @brief C++ Wrapper for LVGL Scale Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Scale widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl

#endif  // LV_USE_SCALE
#endif  // LVGL_CPP_WIDGETS_SCALE_H_
