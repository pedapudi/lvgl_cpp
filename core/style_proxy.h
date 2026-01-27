#ifndef LVGL_CPP_CORE_STYLE_PROXY_H_
#define LVGL_CPP_CORE_STYLE_PROXY_H_

#include <cstdint>

#include "../misc/color.h"
#include "../misc/enums.h"
#include "../misc/style_base.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Proxy for setting local style properties on an object.
 *
 * StyleProxy allows setting style properties directly on an object for a
 * specific part and state (selector). It provides the same fluent API as
 * the Style class.
 */
class StyleProxy : public StyleBase<StyleProxy> {
 public:
  /**
   * @brief Construct a StyleProxy for an object.
   * @param obj Target object.
   * @param selector Part and state selector.
   */
  StyleProxy(lv_obj_t* obj, lv_style_selector_t selector)
      : obj_(obj), selector_(selector) {}

 private:
  friend class StyleBase<StyleProxy>;

  // Internal setters called by StyleBase
  void set_bg_color(lv_color_t v) {
    lv_obj_set_style_bg_color(obj_, v, selector_);
  }
  void set_bg_opa(lv_opa_t v) { lv_obj_set_style_bg_opa(obj_, v, selector_); }
  void set_bg_grad_color(lv_color_t v) {
    lv_obj_set_style_bg_grad_color(obj_, v, selector_);
  }
  void set_bg_grad_dir(lv_grad_dir_t v) {
    lv_obj_set_style_bg_grad_dir(obj_, v, selector_);
  }
  void set_bg_grad_stop(int32_t v) {
    lv_obj_set_style_bg_grad_stop(obj_, v, selector_);
  }
  void set_bg_main_stop(int32_t v) {
    lv_obj_set_style_bg_main_stop(obj_, v, selector_);
  }
  void set_bg_image_src(const void* v) {
    lv_obj_set_style_bg_image_src(obj_, v, selector_);
  }
  void set_bg_image_opa(lv_opa_t v) {
    lv_obj_set_style_bg_image_opa(obj_, v, selector_);
  }
  void set_bg_image_recolor(lv_color_t v) {
    lv_obj_set_style_bg_image_recolor(obj_, v, selector_);
  }
  void set_bg_image_recolor_opa(lv_opa_t v) {
    lv_obj_set_style_bg_image_recolor_opa(obj_, v, selector_);
  }
  void set_bg_image_tiled(bool v) {
    lv_obj_set_style_bg_image_tiled(obj_, v, selector_);
  }

  void set_border_color(lv_color_t v) {
    lv_obj_set_style_border_color(obj_, v, selector_);
  }
  void set_border_width(int32_t v) {
    lv_obj_set_style_border_width(obj_, v, selector_);
  }
  void set_border_opa(lv_opa_t v) {
    lv_obj_set_style_border_opa(obj_, v, selector_);
  }
  void set_border_side(lv_border_side_t v) {
    lv_obj_set_style_border_side(obj_, v, selector_);
  }
  void set_border_post(bool v) {
    lv_obj_set_style_border_post(obj_, v, selector_);
  }

  void set_outline_color(lv_color_t v) {
    lv_obj_set_style_outline_color(obj_, v, selector_);
  }
  void set_outline_width(int32_t v) {
    lv_obj_set_style_outline_width(obj_, v, selector_);
  }
  void set_outline_opa(lv_opa_t v) {
    lv_obj_set_style_outline_opa(obj_, v, selector_);
  }
  void set_outline_pad(int32_t v) {
    lv_obj_set_style_outline_pad(obj_, v, selector_);
  }

  void set_shadow_width(int32_t v) {
    lv_obj_set_style_shadow_width(obj_, v, selector_);
  }
  void set_shadow_color(lv_color_t v) {
    lv_obj_set_style_shadow_color(obj_, v, selector_);
  }
  void set_shadow_opa(lv_opa_t v) {
    lv_obj_set_style_shadow_opa(obj_, v, selector_);
  }
  void set_shadow_offset_x(int32_t v) {
    lv_obj_set_style_shadow_offset_x(obj_, v, selector_);
  }
  void set_shadow_offset_y(int32_t v) {
    lv_obj_set_style_shadow_offset_y(obj_, v, selector_);
  }
  void set_shadow_spread(int32_t v) {
    lv_obj_set_style_shadow_spread(obj_, v, selector_);
  }

  void set_pad_all(int32_t v) { lv_obj_set_style_pad_all(obj_, v, selector_); }
  void set_pad_hor(int32_t v) { lv_obj_set_style_pad_hor(obj_, v, selector_); }
  void set_pad_ver(int32_t v) { lv_obj_set_style_pad_ver(obj_, v, selector_); }
  void set_pad_left(int32_t v) {
    lv_obj_set_style_pad_left(obj_, v, selector_);
  }
  void set_pad_right(int32_t v) {
    lv_obj_set_style_pad_right(obj_, v, selector_);
  }
  void set_pad_top(int32_t v) { lv_obj_set_style_pad_top(obj_, v, selector_); }
  void set_pad_bottom(int32_t v) {
    lv_obj_set_style_pad_bottom(obj_, v, selector_);
  }
  void set_pad_row(int32_t v) { lv_obj_set_style_pad_row(obj_, v, selector_); }
  void set_pad_column(int32_t v) {
    lv_obj_set_style_pad_column(obj_, v, selector_);
  }
  void set_pad_gap(int32_t v) { lv_obj_set_style_pad_gap(obj_, v, selector_); }

  void set_width(int32_t v) { lv_obj_set_style_width(obj_, v, selector_); }
  void set_min_width(int32_t v) {
    lv_obj_set_style_min_width(obj_, v, selector_);
  }
  void set_max_width(int32_t v) {
    lv_obj_set_style_max_width(obj_, v, selector_);
  }
  void set_height(int32_t v) { lv_obj_set_style_height(obj_, v, selector_); }
  void set_min_height(int32_t v) {
    lv_obj_set_style_min_height(obj_, v, selector_);
  }
  void set_max_height(int32_t v) {
    lv_obj_set_style_max_height(obj_, v, selector_);
  }

  void set_radius(int32_t v) { lv_obj_set_style_radius(obj_, v, selector_); }
  void set_clip_corner(bool v) {
    lv_obj_set_style_clip_corner(obj_, v, selector_);
  }
  void set_transform_width(int32_t v) {
    lv_obj_set_style_transform_width(obj_, v, selector_);
  }
  void set_transform_height(int32_t v) {
    lv_obj_set_style_transform_height(obj_, v, selector_);
  }
  void set_translate_x(int32_t v) {
    lv_obj_set_style_translate_x(obj_, v, selector_);
  }
  void set_translate_y(int32_t v) {
    lv_obj_set_style_translate_y(obj_, v, selector_);
  }
  void set_transform_scale_x(int32_t v) {
    lv_obj_set_style_transform_scale_x(obj_, v, selector_);
  }
  void set_transform_scale_y(int32_t v) {
    lv_obj_set_style_transform_scale_y(obj_, v, selector_);
  }
  void set_transform_rotation(int32_t v) {
    lv_obj_set_style_transform_rotation(obj_, v, selector_);
  }
  void set_transform_pivot_x(int32_t v) {
    lv_obj_set_style_transform_pivot_x(obj_, v, selector_);
  }
  void set_transform_pivot_y(int32_t v) {
    lv_obj_set_style_transform_pivot_y(obj_, v, selector_);
  }

  void set_text_color(lv_color_t v) {
    lv_obj_set_style_text_color(obj_, v, selector_);
  }
  void set_text_opa(lv_opa_t v) {
    lv_obj_set_style_text_opa(obj_, v, selector_);
  }
  void set_text_font(const lv_font_t* v) {
    lv_obj_set_style_text_font(obj_, v, selector_);
  }
  void set_text_letter_space(int32_t v) {
    lv_obj_set_style_text_letter_space(obj_, v, selector_);
  }
  void set_text_line_space(int32_t v) {
    lv_obj_set_style_text_line_space(obj_, v, selector_);
  }
  void set_text_align(lv_text_align_t v) {
    lv_obj_set_style_text_align(obj_, v, selector_);
  }
  void set_text_decor(lv_text_decor_t v) {
    lv_obj_set_style_text_decor(obj_, v, selector_);
  }

  void set_image_opa(lv_opa_t v) {
    lv_obj_set_style_image_opa(obj_, v, selector_);
  }
  void set_image_recolor(lv_color_t v) {
    lv_obj_set_style_image_recolor(obj_, v, selector_);
  }
  void set_image_recolor_opa(lv_opa_t v) {
    lv_obj_set_style_image_recolor_opa(obj_, v, selector_);
  }

  void set_line_width(int32_t v) {
    lv_obj_set_style_line_width(obj_, v, selector_);
  }
  void set_line_dash_width(int32_t v) {
    lv_obj_set_style_line_dash_width(obj_, v, selector_);
  }
  void set_line_dash_gap(int32_t v) {
    lv_obj_set_style_line_dash_gap(obj_, v, selector_);
  }
  void set_line_rounded(bool v) {
    lv_obj_set_style_line_rounded(obj_, v, selector_);
  }
  void set_line_color(lv_color_t v) {
    lv_obj_set_style_line_color(obj_, v, selector_);
  }
  void set_line_opa(lv_opa_t v) {
    lv_obj_set_style_line_opa(obj_, v, selector_);
  }

  void set_arc_width(int32_t v) {
    lv_obj_set_style_arc_width(obj_, v, selector_);
  }
  void set_arc_rounded(bool v) {
    lv_obj_set_style_arc_rounded(obj_, v, selector_);
  }
  void set_arc_color(lv_color_t v) {
    lv_obj_set_style_arc_color(obj_, v, selector_);
  }
  void set_arc_opa(lv_opa_t v) { lv_obj_set_style_arc_opa(obj_, v, selector_); }
  void set_arc_image_src(const void* v) {
    lv_obj_set_style_arc_image_src(obj_, v, selector_);
  }

  void set_anim_duration(uint32_t v) {
    lv_obj_set_style_anim_duration(obj_, v, selector_);
  }
  void set_transition(const lv_style_transition_dsc_t* v) {
    lv_obj_set_style_transition(obj_, v, selector_);
  }

  void set_flex_flow(lv_flex_flow_t v) {
    lv_obj_set_style_flex_flow(obj_, v, selector_);
  }
  void set_flex_main_place(lv_flex_align_t v) {
    lv_obj_set_style_flex_main_place(obj_, v, selector_);
  }
  void set_flex_cross_place(lv_flex_align_t v) {
    lv_obj_set_style_flex_cross_place(obj_, v, selector_);
  }
  void set_flex_track_place(lv_flex_align_t v) {
    lv_obj_set_style_flex_track_place(obj_, v, selector_);
  }
  void set_flex_grow(uint8_t v) {
    lv_obj_set_style_flex_grow(obj_, v, selector_);
  }

  void set_grid_row_dsc_array(const int32_t* v) {
    lv_obj_set_style_grid_row_dsc_array(obj_, v, selector_);
  }
  void set_grid_column_dsc_array(const int32_t* v) {
    lv_obj_set_style_grid_column_dsc_array(obj_, v, selector_);
  }
  void set_grid_row_align(lv_grid_align_t v) {
    lv_obj_set_style_grid_row_align(obj_, v, selector_);
  }
  void set_grid_column_align(lv_grid_align_t v) {
    lv_obj_set_style_grid_column_align(obj_, v, selector_);
  }
  void set_grid_cell_column_pos(int32_t v) {
    lv_obj_set_style_grid_cell_column_pos(obj_, v, selector_);
  }
  void set_grid_cell_column_span(int32_t v) {
    lv_obj_set_style_grid_cell_column_span(obj_, v, selector_);
  }
  void set_grid_cell_row_pos(int32_t v) {
    lv_obj_set_style_grid_cell_row_pos(obj_, v, selector_);
  }
  void set_grid_cell_row_span(int32_t v) {
    lv_obj_set_style_grid_cell_row_span(obj_, v, selector_);
  }
  void set_grid_cell_x_align(lv_grid_align_t v) {
    lv_obj_set_style_grid_cell_x_align(obj_, v, selector_);
  }
  void set_grid_cell_y_align(lv_grid_align_t v) {
    lv_obj_set_style_grid_cell_y_align(obj_, v, selector_);
  }

  void set_opa(lv_opa_t v) { lv_obj_set_style_opa(obj_, v, selector_); }
  void set_blend_mode(lv_blend_mode_t v) {
    lv_obj_set_style_blend_mode(obj_, v, selector_);
  }
  void set_base_dir(lv_base_dir_t v) {
    lv_obj_set_style_base_dir(obj_, v, selector_);
  }

  lv_obj_t* obj_;
  lv_style_selector_t selector_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_STYLE_PROXY_H_
