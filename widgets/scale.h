#ifndef LVGL_CPP_WIDGETS_SCALE_H_
#define LVGL_CPP_WIDGETS_SCALE_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
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

class Scale;  // Forward declaration

/**
 * @brief Wrapper for a scale section.
 */
class ScaleSection {
 public:
  ScaleSection(Scale* scale, lv_scale_section_t* section)
      : scale_(scale), section_(section) {}

  /**
   * @brief Set the range of values for this section.
   */
  void set_range(int32_t min, int32_t max);

  /**
   * @brief Set the style for a specific part of this section.
   */
  void set_style(Part part, Style& style);

  /**
   * @brief Set the style for a specific part of this section (Raw).
   */
  void set_style(lv_part_t part, lv_style_t* style);

  lv_scale_section_t* raw() const { return section_; }

 private:
  Scale* scale_;
  lv_scale_section_t* section_;
};

class Scale : public Widget<Scale> {
 public:
  /**
   * @brief Scale display modes.
   */
  enum class Mode : uint8_t {
    HorizontalTop = LV_SCALE_MODE_HORIZONTAL_TOP,
    HorizontalBottom = LV_SCALE_MODE_HORIZONTAL_BOTTOM,
    VerticalLeft = LV_SCALE_MODE_VERTICAL_LEFT,
    VerticalRight = LV_SCALE_MODE_VERTICAL_RIGHT,
    RoundInner = LV_SCALE_MODE_ROUND_INNER,
    RoundOuter = LV_SCALE_MODE_ROUND_OUTER,
  };

  Scale();
  explicit Scale(Object* parent, Ownership ownership = Ownership::Default);
  explicit Scale(Object& parent);
  explicit Scale(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Scale& set_mode(Mode mode);
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

  Mode get_mode();

  // Section management
  ScaleSection add_section();
};

}  // namespace lvgl

#endif  // LV_USE_SCALE
#endif  // LVGL_CPP_WIDGETS_SCALE_H_
