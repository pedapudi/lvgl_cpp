#include "style.h"

#include <utility>

namespace lvgl {

Style::Style() : initialized_(true) { lv_style_init(&style_); }

Style::~Style() {
  if (initialized_) {
    lv_style_reset(&style_);
  }
}

Style::Style(Style&& other) noexcept
    : style_(other.style_), initialized_(other.initialized_) {
  other.initialized_ = false;
}

Style& Style::operator=(Style&& other) noexcept {
  if (this != &other) {
    if (initialized_) {
      lv_style_reset(&style_);
    }
    style_ = other.style_;
    initialized_ = other.initialized_;
    other.initialized_ = false;
  }
  return *this;
}

// =========================================================================
// Background
// =========================================================================

Style& Style::bg_color(Color color) {
  lv_style_set_bg_color(&style_, color);
  return *this;
}

Style& Style::bg_color(lv_color_t color) {
  lv_style_set_bg_color(&style_, color);
  return *this;
}

Style& Style::bg_opa(Opacity opa) {
  lv_style_set_bg_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::bg_opa(lv_opa_t opa) {
  lv_style_set_bg_opa(&style_, opa);
  return *this;
}

Style& Style::bg_grad_color(Color color) {
  lv_style_set_bg_grad_color(&style_, color);
  return *this;
}

Style& Style::bg_grad_dir(lv_grad_dir_t dir) {
  lv_style_set_bg_grad_dir(&style_, dir);
  return *this;
}

Style& Style::bg_grad_stop(int32_t value) {
  lv_style_set_bg_grad_stop(&style_, value);
  return *this;
}

Style& Style::bg_main_stop(int32_t value) {
  lv_style_set_bg_main_stop(&style_, value);
  return *this;
}

Style& Style::bg_image_src(const void* src) {
  lv_style_set_bg_image_src(&style_, src);
  return *this;
}

Style& Style::bg_image_opa(Opacity opa) {
  lv_style_set_bg_image_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::bg_image_opa(lv_opa_t opa) {
  lv_style_set_bg_image_opa(&style_, opa);
  return *this;
}

Style& Style::bg_image_recolor(Color color) {
  lv_style_set_bg_image_recolor(&style_, color);
  return *this;
}

Style& Style::bg_image_recolor(lv_color_t color) {
  lv_style_set_bg_image_recolor(&style_, color);
  return *this;
}

Style& Style::bg_image_recolor_opa(Opacity opa) {
  lv_style_set_bg_image_recolor_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::bg_image_recolor_opa(lv_opa_t opa) {
  lv_style_set_bg_image_recolor_opa(&style_, opa);
  return *this;
}

Style& Style::bg_image_tiled(bool tiled) {
  lv_style_set_bg_image_tiled(&style_, tiled);
  return *this;
}

// =========================================================================
// Border
// =========================================================================

Style& Style::border_color(Color color) {
  lv_style_set_border_color(&style_, color);
  return *this;
}

Style& Style::border_color(lv_color_t color) {
  lv_style_set_border_color(&style_, color);
  return *this;
}

Style& Style::border_width(int32_t width) {
  lv_style_set_border_width(&style_, width);
  return *this;
}

Style& Style::border_opa(Opacity opa) {
  lv_style_set_border_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::border_opa(lv_opa_t opa) {
  lv_style_set_border_opa(&style_, opa);
  return *this;
}

Style& Style::border_side(BorderSide side) {
  lv_style_set_border_side(&style_, static_cast<lv_border_side_t>(side));
  return *this;
}

Style& Style::border_side(lv_border_side_t side) {
  lv_style_set_border_side(&style_, side);
  return *this;
}

Style& Style::border_post(bool post) {
  lv_style_set_border_post(&style_, post);
  return *this;
}

// =========================================================================
// Outline
// =========================================================================

Style& Style::outline_color(Color color) {
  lv_style_set_outline_color(&style_, color);
  return *this;
}

Style& Style::outline_color(lv_color_t color) {
  lv_style_set_outline_color(&style_, color);
  return *this;
}

Style& Style::outline_width(int32_t width) {
  lv_style_set_outline_width(&style_, width);
  return *this;
}

Style& Style::outline_opa(Opacity opa) {
  lv_style_set_outline_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::outline_opa(lv_opa_t opa) {
  lv_style_set_outline_opa(&style_, opa);
  return *this;
}

Style& Style::outline_pad(int32_t pad) {
  lv_style_set_outline_pad(&style_, pad);
  return *this;
}

// =========================================================================
// Padding
// =========================================================================

Style& Style::pad_all(int32_t pad) {
  lv_style_set_pad_all(&style_, pad);
  return *this;
}

Style& Style::pad_hor(int32_t pad) {
  lv_style_set_pad_hor(&style_, pad);
  return *this;
}

Style& Style::pad_ver(int32_t pad) {
  lv_style_set_pad_ver(&style_, pad);
  return *this;
}

Style& Style::pad_left(int32_t pad) {
  lv_style_set_pad_left(&style_, pad);
  return *this;
}

Style& Style::pad_right(int32_t pad) {
  lv_style_set_pad_right(&style_, pad);
  return *this;
}

Style& Style::pad_top(int32_t pad) {
  lv_style_set_pad_top(&style_, pad);
  return *this;
}

Style& Style::pad_bottom(int32_t pad) {
  lv_style_set_pad_bottom(&style_, pad);
  return *this;
}

Style& Style::pad_gap(int32_t gap) {
  lv_style_set_pad_gap(&style_, gap);
  return *this;
}

Style& Style::pad_row(int32_t pad) {
  lv_style_set_pad_row(&style_, pad);
  return *this;
}

Style& Style::pad_column(int32_t pad) {
  lv_style_set_pad_column(&style_, pad);
  return *this;
}

// =========================================================================
// Size
// =========================================================================

Style& Style::width(int32_t value) {
  lv_style_set_width(&style_, value);
  return *this;
}

Style& Style::min_width(int32_t value) {
  lv_style_set_min_width(&style_, value);
  return *this;
}

Style& Style::max_width(int32_t value) {
  lv_style_set_max_width(&style_, value);
  return *this;
}

Style& Style::height(int32_t value) {
  lv_style_set_height(&style_, value);
  return *this;
}

Style& Style::min_height(int32_t value) {
  lv_style_set_min_height(&style_, value);
  return *this;
}

Style& Style::max_height(int32_t value) {
  lv_style_set_max_height(&style_, value);
  return *this;
}

// =========================================================================
// Geometry
// =========================================================================

Style& Style::radius(int32_t radius) {
  lv_style_set_radius(&style_, radius);
  return *this;
}

Style& Style::clip_corner(bool clip) {
  lv_style_set_clip_corner(&style_, clip);
  return *this;
}

Style& Style::transform_width(int32_t value) {
  lv_style_set_transform_width(&style_, value);
  return *this;
}

Style& Style::transform_height(int32_t value) {
  lv_style_set_transform_height(&style_, value);
  return *this;
}

Style& Style::translate_x(int32_t value) {
  lv_style_set_translate_x(&style_, value);
  return *this;
}

Style& Style::translate_y(int32_t value) {
  lv_style_set_translate_y(&style_, value);
  return *this;
}

Style& Style::transform_scale_x(int32_t value) {
  lv_style_set_transform_scale_x(&style_, value);
  return *this;
}

Style& Style::transform_scale_y(int32_t value) {
  lv_style_set_transform_scale_y(&style_, value);
  return *this;
}

Style& Style::transform_rotation(int32_t value) {
  lv_style_set_transform_rotation(&style_, value);
  return *this;
}

// =========================================================================
// Text
// =========================================================================

Style& Style::text_color(Color color) {
  lv_style_set_text_color(&style_, color);
  return *this;
}

Style& Style::text_color(lv_color_t color) {
  lv_style_set_text_color(&style_, color);
  return *this;
}

Style& Style::text_opa(Opacity opa) {
  lv_style_set_text_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::text_opa(lv_opa_t opa) {
  lv_style_set_text_opa(&style_, opa);
  return *this;
}

Style& Style::text_font(const lv_font_t* font) {
  lv_style_set_text_font(&style_, font);
  return *this;
}

Style& Style::text_letter_space(int32_t space) {
  lv_style_set_text_letter_space(&style_, space);
  return *this;
}

Style& Style::text_line_space(int32_t space) {
  lv_style_set_text_line_space(&style_, space);
  return *this;
}

Style& Style::text_align(TextAlign align) {
  lv_style_set_text_align(&style_, static_cast<lv_text_align_t>(align));
  return *this;
}

Style& Style::text_align(lv_text_align_t align) {
  lv_style_set_text_align(&style_, align);
  return *this;
}

Style& Style::text_decor(TextDecor decor) {
  lv_style_set_text_decor(&style_, static_cast<lv_text_decor_t>(decor));
  return *this;
}

Style& Style::text_decor(lv_text_decor_t decor) {
  lv_style_set_text_decor(&style_, decor);
  return *this;
}

// =========================================================================
// Image
// =========================================================================

Style& Style::image_opa(Opacity opa) {
  lv_style_set_image_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::image_opa(lv_opa_t opa) {
  lv_style_set_image_opa(&style_, opa);
  return *this;
}

Style& Style::image_recolor(Color color) {
  lv_style_set_image_recolor(&style_, color);
  return *this;
}

Style& Style::image_recolor(lv_color_t color) {
  lv_style_set_image_recolor(&style_, color);
  return *this;
}

Style& Style::image_recolor_opa(Opacity opa) {
  lv_style_set_image_recolor_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::image_recolor_opa(lv_opa_t opa) {
  lv_style_set_image_recolor_opa(&style_, opa);
  return *this;
}

// =========================================================================
// Line
// =========================================================================

Style& Style::line_width(int32_t width) {
  lv_style_set_line_width(&style_, width);
  return *this;
}

Style& Style::line_dash_width(int32_t width) {
  lv_style_set_line_dash_width(&style_, width);
  return *this;
}

Style& Style::line_dash_gap(int32_t gap) {
  lv_style_set_line_dash_gap(&style_, gap);
  return *this;
}

Style& Style::line_rounded(bool rounded) {
  lv_style_set_line_rounded(&style_, rounded);
  return *this;
}

Style& Style::line_color(Color color) {
  lv_style_set_line_color(&style_, color);
  return *this;
}

Style& Style::line_color(lv_color_t color) {
  lv_style_set_line_color(&style_, color);
  return *this;
}

Style& Style::line_opa(Opacity opa) {
  lv_style_set_line_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::line_opa(lv_opa_t opa) {
  lv_style_set_line_opa(&style_, opa);
  return *this;
}

// =========================================================================
// Arc
// =========================================================================

Style& Style::arc_width(int32_t width) {
  lv_style_set_arc_width(&style_, width);
  return *this;
}

Style& Style::arc_rounded(bool rounded) {
  lv_style_set_arc_rounded(&style_, rounded);
  return *this;
}

Style& Style::arc_color(Color color) {
  lv_style_set_arc_color(&style_, color);
  return *this;
}

Style& Style::arc_color(lv_color_t color) {
  lv_style_set_arc_color(&style_, color);
  return *this;
}

Style& Style::arc_opa(Opacity opa) {
  lv_style_set_arc_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::arc_opa(lv_opa_t opa) {
  lv_style_set_arc_opa(&style_, opa);
  return *this;
}

Style& Style::arc_image_src(const void* src) {
  lv_style_set_arc_image_src(&style_, src);
  return *this;
}

// =========================================================================
// Animation
// =========================================================================

Style& Style::anim_time(uint32_t time) {
  lv_style_set_anim_time(&style_, time);
  return *this;
}

Style& Style::anim_duration(uint32_t duration) {
  lv_style_set_anim_duration(&style_, duration);
  return *this;
}

Style& Style::transition(const lv_style_transition_dsc_t* dsc) {
  lv_style_set_transition(&style_, dsc);
  return *this;
}

// =========================================================================
// Layout (Flex/Grid)
// =========================================================================

Style& Style::flex_flow(lv_flex_flow_t flow) {
  lv_style_set_flex_flow(&style_, flow);
  return *this;
}

Style& Style::flex_main_place(lv_flex_align_t place) {
  lv_style_set_flex_main_place(&style_, place);
  return *this;
}

Style& Style::flex_cross_place(lv_flex_align_t place) {
  lv_style_set_flex_cross_place(&style_, place);
  return *this;
}

Style& Style::flex_track_place(lv_flex_align_t place) {
  lv_style_set_flex_track_place(&style_, place);
  return *this;
}

Style& Style::flex_grow(uint8_t grow) {
  lv_style_set_flex_grow(&style_, grow);
  return *this;
}

Style& Style::grid_row_dsc_array(const int32_t* dsc) {
  lv_style_set_grid_row_dsc_array(&style_, dsc);
  return *this;
}

Style& Style::grid_column_dsc_array(const int32_t* dsc) {
  lv_style_set_grid_column_dsc_array(&style_, dsc);
  return *this;
}

Style& Style::grid_row_align(lv_grid_align_t align) {
  lv_style_set_grid_row_align(&style_, align);
  return *this;
}

Style& Style::grid_column_align(lv_grid_align_t align) {
  lv_style_set_grid_column_align(&style_, align);
  return *this;
}

Style& Style::grid_cell_column_pos(int32_t pos) {
  lv_style_set_grid_cell_column_pos(&style_, pos);
  return *this;
}

Style& Style::grid_cell_column_span(int32_t span) {
  lv_style_set_grid_cell_column_span(&style_, span);
  return *this;
}

Style& Style::grid_cell_row_pos(int32_t pos) {
  lv_style_set_grid_cell_row_pos(&style_, pos);
  return *this;
}

Style& Style::grid_cell_row_span(int32_t span) {
  lv_style_set_grid_cell_row_span(&style_, span);
  return *this;
}

Style& Style::grid_cell_x_align(lv_grid_align_t align) {
  lv_style_set_grid_cell_x_align(&style_, align);
  return *this;
}

Style& Style::grid_cell_y_align(lv_grid_align_t align) {
  lv_style_set_grid_cell_y_align(&style_, align);
  return *this;
}

// =========================================================================
// Shadow
// =========================================================================

Style& Style::shadow_color(Color color) {
  lv_style_set_shadow_color(&style_, color);
  return *this;
}

Style& Style::shadow_color(lv_color_t color) {
  lv_style_set_shadow_color(&style_, color);
  return *this;
}

Style& Style::shadow_opa(Opacity opa) {
  lv_style_set_shadow_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::shadow_opa(lv_opa_t opa) {
  lv_style_set_shadow_opa(&style_, opa);
  return *this;
}

Style& Style::shadow_width(int32_t width) {
  lv_style_set_shadow_width(&style_, width);
  return *this;
}

Style& Style::shadow_offset_x(int32_t offset) {
  lv_style_set_shadow_offset_x(&style_, offset);
  return *this;
}

Style& Style::shadow_offset_y(int32_t offset) {
  lv_style_set_shadow_offset_y(&style_, offset);
  return *this;
}

Style& Style::shadow_spread(int32_t spread) {
  lv_style_set_shadow_spread(&style_, spread);
  return *this;
}

// =========================================================================
// Miscellaneous
// =========================================================================

Style& Style::opa(Opacity opa) {
  lv_style_set_opa(&style_, static_cast<lv_opa_t>(opa));
  return *this;
}

Style& Style::opa(lv_opa_t opa) {
  lv_style_set_opa(&style_, opa);
  return *this;
}

Style& Style::blend_mode(BlendMode mode) {
  lv_style_set_blend_mode(&style_, static_cast<lv_blend_mode_t>(mode));
  return *this;
}

Style& Style::blend_mode(lv_blend_mode_t mode) {
  lv_style_set_blend_mode(&style_, mode);
  return *this;
}

Style& Style::base_dir(lv_base_dir_t dir) {
  lv_style_set_base_dir(&style_, dir);
  return *this;
}

// =========================================================================
// Raw access
// =========================================================================

lv_style_t* Style::raw() { return &style_; }

const lv_style_t* Style::raw() const { return &style_; }

}  // namespace lvgl
