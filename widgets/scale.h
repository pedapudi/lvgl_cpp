#ifndef LVGL_CPP_WIDGETS_SCALE_H_
#define LVGL_CPP_WIDGETS_SCALE_H_

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
  explicit Scale(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Scale(lv_obj_t* obj);

  void set_mode(lv_scale_mode_t mode);
  void set_total_tick_count(uint32_t total_tick_count);
  void set_major_tick_every(uint32_t major_tick_every);
  void set_label_show(bool show_label);
  void set_range(int32_t min, int32_t max);
  void set_angle_range(uint32_t angle_range);
  void set_rotation(int32_t rotation);
  void set_line_needle_value(lv_obj_t* needle_line, int32_t needle_length,
                             int32_t value);
  void set_image_needle_value(lv_obj_t* needle_img, int32_t value);
  void set_text_src(const char* txt_src[]);
  void set_post_draw(bool en);
  void set_draw_ticks_on_top(bool en);

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


#endif // LV_USE_SCALE
#endif  // LVGL_CPP_WIDGETS_SCALE_H_
