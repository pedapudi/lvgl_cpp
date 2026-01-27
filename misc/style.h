#ifndef LVGL_CPP_MISC_STYLE_H_
#define LVGL_CPP_MISC_STYLE_H_

#include <cstdint>

#include "color.h"
#include "enums.h"
#include "lvgl.h"
#include "style_base.h"

namespace lvgl {

class Font;

/**
 * @brief Fluent wrapper for lv_style_t.
 *
 * The Style class provides a fluent API for defining LVGL styles. It owns
 * the underlying lv_style_t and automatically initializes it on construction.
 */
class Style : public StyleBase<Style> {
 public:
  /**
   * @brief Construct and initialize an empty style.
   */
  Style();

  /**
   * @brief Destructor. Resets the style.
   */
  ~Style();

  // Non-copyable (lv_style_t has internal pointers)
  Style(const Style&) = delete;
  Style& operator=(const Style&) = delete;

  // Movable
  Style(Style&& other) noexcept;
  Style& operator=(Style&& other) noexcept;

  // =========================================================================
  // Raw access
  // =========================================================================

  /**
   * @brief Get the raw lv_style_t pointer (non-const).
   */
  lv_style_t* raw();

  /**
   * @brief Get the raw lv_style_t pointer (const).
   */
  const lv_style_t* raw() const;

 private:
  friend class StyleBase<Style>;

  // Internal setters called by StyleBase
  void set_bg_color(lv_color_t v) { lv_style_set_bg_color(&style_, v); }
  void set_bg_opa(lv_opa_t v) { lv_style_set_bg_opa(&style_, v); }
  void set_bg_grad_color(lv_color_t v) {
    lv_style_set_bg_grad_color(&style_, v);
  }
  void set_bg_grad_dir(lv_grad_dir_t v) {
    lv_style_set_bg_grad_dir(&style_, v);
  }
  void set_bg_grad_stop(int32_t v) { lv_style_set_bg_grad_stop(&style_, v); }
  void set_bg_main_stop(int32_t v) { lv_style_set_bg_main_stop(&style_, v); }
  void set_bg_image_src(const void* v) {
    lv_style_set_bg_image_src(&style_, v);
  }
  void set_bg_image_opa(lv_opa_t v) { lv_style_set_bg_image_opa(&style_, v); }
  void set_bg_image_recolor(lv_color_t v) {
    lv_style_set_bg_image_recolor(&style_, v);
  }
  void set_bg_image_recolor_opa(lv_opa_t v) {
    lv_style_set_bg_image_recolor_opa(&style_, v);
  }
  void set_bg_image_tiled(bool v) { lv_style_set_bg_image_tiled(&style_, v); }

  void set_border_color(lv_color_t v) { lv_style_set_border_color(&style_, v); }
  void set_border_width(int32_t v) { lv_style_set_border_width(&style_, v); }
  void set_border_opa(lv_opa_t v) { lv_style_set_border_opa(&style_, v); }
  void set_border_side(lv_border_side_t v) {
    lv_style_set_border_side(&style_, v);
  }
  void set_border_post(bool v) { lv_style_set_border_post(&style_, v); }

  void set_outline_color(lv_color_t v) {
    lv_style_set_outline_color(&style_, v);
  }
  void set_outline_width(int32_t v) { lv_style_set_outline_width(&style_, v); }
  void set_outline_opa(lv_opa_t v) { lv_style_set_outline_opa(&style_, v); }
  void set_outline_pad(int32_t v) { lv_style_set_outline_pad(&style_, v); }

  void set_shadow_width(int32_t v) { lv_style_set_shadow_width(&style_, v); }
  void set_shadow_color(lv_color_t v) { lv_style_set_shadow_color(&style_, v); }
  void set_shadow_opa(lv_opa_t v) { lv_style_set_shadow_opa(&style_, v); }
  void set_shadow_offset_x(int32_t v) {
    lv_style_set_shadow_offset_x(&style_, v);
  }
  void set_shadow_offset_y(int32_t v) {
    lv_style_set_shadow_offset_y(&style_, v);
  }
  void set_shadow_spread(int32_t v) { lv_style_set_shadow_spread(&style_, v); }

  void set_pad_all(int32_t v) { lv_style_set_pad_all(&style_, v); }
  void set_pad_hor(int32_t v) { lv_style_set_pad_hor(&style_, v); }
  void set_pad_ver(int32_t v) { lv_style_set_pad_ver(&style_, v); }
  void set_pad_left(int32_t v) { lv_style_set_pad_left(&style_, v); }
  void set_pad_right(int32_t v) { lv_style_set_pad_right(&style_, v); }
  void set_pad_top(int32_t v) { lv_style_set_pad_top(&style_, v); }
  void set_pad_bottom(int32_t v) { lv_style_set_pad_bottom(&style_, v); }
  void set_pad_row(int32_t v) { lv_style_set_pad_row(&style_, v); }
  void set_pad_column(int32_t v) { lv_style_set_pad_column(&style_, v); }
  void set_pad_gap(int32_t v) { lv_style_set_pad_gap(&style_, v); }

  void set_width(int32_t v) { lv_style_set_width(&style_, v); }
  void set_min_width(int32_t v) { lv_style_set_min_width(&style_, v); }
  void set_max_width(int32_t v) { lv_style_set_max_width(&style_, v); }
  void set_height(int32_t v) { lv_style_set_height(&style_, v); }
  void set_min_height(int32_t v) { lv_style_set_min_height(&style_, v); }
  void set_max_height(int32_t v) { lv_style_set_max_height(&style_, v); }

  void set_radius(int32_t v) { lv_style_set_radius(&style_, v); }
  void set_clip_corner(bool v) { lv_style_set_clip_corner(&style_, v); }
  void set_transform_width(int32_t v) {
    lv_style_set_transform_width(&style_, v);
  }
  void set_transform_height(int32_t v) {
    lv_style_set_transform_height(&style_, v);
  }
  void set_translate_x(int32_t v) { lv_style_set_translate_x(&style_, v); }
  void set_translate_y(int32_t v) { lv_style_set_translate_y(&style_, v); }
  void set_transform_scale_x(int32_t v) {
    lv_style_set_transform_scale_x(&style_, v);
  }
  void set_transform_scale_y(int32_t v) {
    lv_style_set_transform_scale_y(&style_, v);
  }
  void set_transform_rotation(int32_t v) {
    lv_style_set_transform_rotation(&style_, v);
  }
  void set_transform_pivot_x(int32_t v) {
    lv_style_set_transform_pivot_x(&style_, v);
  }
  void set_transform_pivot_y(int32_t v) {
    lv_style_set_transform_pivot_y(&style_, v);
  }

  void set_text_color(lv_color_t v) { lv_style_set_text_color(&style_, v); }
  void set_text_opa(lv_opa_t v) { lv_style_set_text_opa(&style_, v); }
  void set_text_font(const lv_font_t* v) { lv_style_set_text_font(&style_, v); }
  void set_text_letter_space(int32_t v) {
    lv_style_set_text_letter_space(&style_, v);
  }
  void set_text_line_space(int32_t v) {
    lv_style_set_text_line_space(&style_, v);
  }
  void set_text_align(lv_text_align_t v) {
    lv_style_set_text_align(&style_, v);
  }
  void set_text_decor(lv_text_decor_t v) {
    lv_style_set_text_decor(&style_, v);
  }

  void set_image_opa(lv_opa_t v) { lv_style_set_image_opa(&style_, v); }
  void set_image_recolor(lv_color_t v) {
    lv_style_set_image_recolor(&style_, v);
  }
  void set_image_recolor_opa(lv_opa_t v) {
    lv_style_set_image_recolor_opa(&style_, v);
  }

  void set_line_width(int32_t v) { lv_style_set_line_width(&style_, v); }
  void set_line_dash_width(int32_t v) {
    lv_style_set_line_dash_width(&style_, v);
  }
  void set_line_dash_gap(int32_t v) { lv_style_set_line_dash_gap(&style_, v); }
  void set_line_rounded(bool v) { lv_style_set_line_rounded(&style_, v); }
  void set_line_color(lv_color_t v) { lv_style_set_line_color(&style_, v); }
  void set_line_opa(lv_opa_t v) { lv_style_set_line_opa(&style_, v); }

  void set_arc_width(int32_t v) { lv_style_set_arc_width(&style_, v); }
  void set_arc_rounded(bool v) { lv_style_set_arc_rounded(&style_, v); }
  void set_arc_color(lv_color_t v) { lv_style_set_arc_color(&style_, v); }
  void set_arc_opa(lv_opa_t v) { lv_style_set_arc_opa(&style_, v); }
  void set_arc_image_src(const void* v) {
    lv_style_set_arc_image_src(&style_, v);
  }

  void set_anim_duration(uint32_t v) { lv_style_set_anim_duration(&style_, v); }
  void set_transition(const lv_style_transition_dsc_t* v) {
    lv_style_set_transition(&style_, v);
  }

  void set_flex_flow(lv_flex_flow_t v) { lv_style_set_flex_flow(&style_, v); }
  void set_flex_main_place(lv_flex_align_t v) {
    lv_style_set_flex_main_place(&style_, v);
  }
  void set_flex_cross_place(lv_flex_align_t v) {
    lv_style_set_flex_cross_place(&style_, v);
  }
  void set_flex_track_place(lv_flex_align_t v) {
    lv_style_set_flex_track_place(&style_, v);
  }
  void set_flex_grow(uint8_t v) { lv_style_set_flex_grow(&style_, v); }

  void set_grid_row_dsc_array(const int32_t* v) {
    lv_style_set_grid_row_dsc_array(&style_, v);
  }
  void set_grid_column_dsc_array(const int32_t* v) {
    lv_style_set_grid_column_dsc_array(&style_, v);
  }
  void set_grid_row_align(lv_grid_align_t v) {
    lv_style_set_grid_row_align(&style_, v);
  }
  void set_grid_column_align(lv_grid_align_t v) {
    lv_style_set_grid_column_align(&style_, v);
  }
  void set_grid_cell_column_pos(int32_t v) {
    lv_style_set_grid_cell_column_pos(&style_, v);
  }
  void set_grid_cell_column_span(int32_t v) {
    lv_style_set_grid_cell_column_span(&style_, v);
  }
  void set_grid_cell_row_pos(int32_t v) {
    lv_style_set_grid_cell_row_pos(&style_, v);
  }
  void set_grid_cell_row_span(int32_t v) {
    lv_style_set_grid_cell_row_span(&style_, v);
  }
  void set_grid_cell_x_align(lv_grid_align_t v) {
    lv_style_set_grid_cell_x_align(&style_, v);
  }
  void set_grid_cell_y_align(lv_grid_align_t v) {
    lv_style_set_grid_cell_y_align(&style_, v);
  }

  void set_opa(lv_opa_t v) { lv_style_set_opa(&style_, v); }
  void set_blend_mode(lv_blend_mode_t v) {
    lv_style_set_blend_mode(&style_, v);
  }
  void set_base_dir(lv_base_dir_t v) { lv_style_set_base_dir(&style_, v); }

  lv_style_t style_;
  bool initialized_ = false;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_STYLE_H_
