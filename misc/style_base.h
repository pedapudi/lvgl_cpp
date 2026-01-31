#pragma once

#include <cstdint>

#include "../draw/image_descriptor.h"
#include "../font/font.h"
#include "color.h"
#include "enums.h"
#include "lvgl.h"

namespace lvgl {

class ImageDescriptor;

/**
 * @brief CRTP base class for style properties.
 *
 * This template provides a unified fluent API for both persistent Style objects
 * and transient StyleProxy objects. It uses the Curiously Recurring Template
 * Pattern (CRTP) to allow methods to return the derived type for chaining.
 *
 * Derived classes must implement the internal `set_property` methods.
 */
template <typename Derived>
class StyleBase {
 public:
  Derived& self() { return static_cast<Derived&>(*this); }

  // =========================================================================
  // Background
  // =========================================================================

  Derived& bg_color(Color color) {
    self().set_bg_color(color);
    return self();
  }
  [[deprecated("Use bg_color(Color) instead")]]
  Derived& bg_color(lv_color_t color) {
    self().set_bg_color(color);
    return self();
  }
  Color bg_color() const { return self().get_bg_color(); }

  Derived& bg_opa(Opacity opa) {
    self().set_bg_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  [[deprecated("Use bg_opa(Opacity) instead")]]
  Derived& bg_opa(lv_opa_t opa) {
    self().set_bg_opa(opa);
    return self();
  }

  Derived& bg_grad_color(Color color) {
    self().set_bg_grad_color(color);
    return self();
  }
  Derived& bg_grad_dir(GradDir dir) {
    self().set_bg_grad_dir(static_cast<lv_grad_dir_t>(dir));
    return self();
  }
  [[deprecated("Use bg_grad_dir(GradDir) instead")]]
  Derived& bg_grad_dir(lv_grad_dir_t dir) {
    self().set_bg_grad_dir(dir);
    return self();
  }
  GradDir bg_grad_dir() const {
    return static_cast<GradDir>(self().get_bg_grad_dir());
  }
  Derived& bg_grad_stop(int32_t value) {
    self().set_bg_grad_stop(value);
    return self();
  }
  Derived& bg_main_stop(int32_t value) {
    self().set_bg_main_stop(value);
    return self();
  }
  Derived& bg_grad(const lv_grad_dsc_t* dsc) {
    self().set_bg_grad(dsc);
    return self();
  }
  Derived& bg_grad_opa(Opacity opa) {
    self().set_bg_grad_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  Derived& bg_main_opa(Opacity opa) {
    self().set_bg_main_opa(static_cast<lv_opa_t>(opa));
    return self();
  }

  Derived& bg_image_src(const void* src) {
    self().set_bg_image_src(src);
    return self();
  }
  Derived& bg_image_src(const ImageDescriptor& dsc) {
    self().set_bg_image_src(dsc.raw());
    return self();
  }
  Derived& bg_image_opa(Opacity opa) {
    self().set_bg_image_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  [[deprecated("Use bg_image_opa(Opacity) instead")]]
  Derived& bg_image_opa(lv_opa_t opa) {
    self().set_bg_image_opa(opa);
    return self();
  }
  Derived& bg_image_recolor(Color color) {
    self().set_bg_image_recolor(color);
    return self();
  }
  Derived& bg_image_recolor_opa(Opacity opa) {
    self().set_bg_image_recolor_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  [[deprecated("Use bg_image_recolor_opa(Opacity) instead")]]
  Derived& bg_image_recolor_opa(lv_opa_t opa) {
    self().set_bg_image_recolor_opa(opa);
    return self();
  }
  Derived& bg_image_tiled(bool tiled) {
    self().set_bg_image_tiled(tiled);
    return self();
  }
  Derived& bitmap_mask_src(const void* src) {
    self().set_bitmap_mask_src(src);
    return self();
  }
  Derived& image_colorkey(Color color) {
    self().set_image_colorkey(color);
    return self();
  }

  // =========================================================================
  // Border
  // =========================================================================

  Derived& border_color(Color color) {
    self().set_border_color(color);
    return self();
  }
  Color border_color() const { return self().get_border_color(); }

  Derived& border_width(int32_t width) {
    self().set_border_width(width);
    return self();
  }
  int32_t border_width() const { return self().get_border_width(); }

  Derived& border_opa(Opacity opa) {
    self().set_border_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  Opacity border_opa() const {
    return static_cast<Opacity>(self().get_border_opa());
  }
  [[deprecated("Use border_opa(Opacity) instead")]]
  Derived& border_opa(lv_opa_t opa) {
    self().set_border_opa(opa);
    return self();
  }
  Derived& border_side(BorderSide side) {
    self().set_border_side(static_cast<lv_border_side_t>(side));
    return self();
  }
  [[deprecated("Use border_side(BorderSide) instead")]]
  Derived& border_side(lv_border_side_t side) {
    self().set_border_side(side);
    return self();
  }
  Derived& border_post(bool post) {
    self().set_border_post(post);
    return self();
  }

  // =========================================================================
  // Outline
  // =========================================================================

  Derived& outline_color(Color color) {
    self().set_outline_color(color);
    return self();
  }
  Derived& outline_width(int32_t width) {
    self().set_outline_width(width);
    return self();
  }
  Derived& outline_opa(Opacity opa) {
    self().set_outline_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  [[deprecated("Use outline_opa(Opacity) instead")]]
  Derived& outline_opa(lv_opa_t opa) {
    self().set_outline_opa(opa);
    return self();
  }
  Derived& outline_pad(int32_t pad) {
    self().set_outline_pad(pad);
    return self();
  }

  // =========================================================================
  // Shadow
  // =========================================================================

  Derived& shadow_width(int32_t width) {
    self().set_shadow_width(width);
    return self();
  }
  Derived& shadow_color(Color color) {
    self().set_shadow_color(color);
    return self();
  }
  Derived& shadow_opa(Opacity opa) {
    self().set_shadow_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  [[deprecated("Use shadow_opa(Opacity) instead")]]
  Derived& shadow_opa(lv_opa_t opa) {
    self().set_shadow_opa(opa);
    return self();
  }
  Derived& shadow_offset_x(int32_t offset) {
    self().set_shadow_offset_x(offset);
    return self();
  }
  Derived& shadow_ofs_x(int32_t offset) { return shadow_offset_x(offset); }

  Derived& shadow_offset_y(int32_t offset) {
    self().set_shadow_offset_y(offset);
    return self();
  }
  Derived& shadow_ofs_y(int32_t offset) { return shadow_offset_y(offset); }
  Derived& shadow_spread(int32_t spread) {
    self().set_shadow_spread(spread);
    return self();
  }

  // =========================================================================
  // Padding
  // =========================================================================

  Derived& pad_all(int32_t pad) {
    self().set_pad_all(pad);
    return self();
  }
  Derived& pad_hor(int32_t pad) {
    self().set_pad_hor(pad);
    return self();
  }
  Derived& pad_ver(int32_t pad) {
    self().set_pad_ver(pad);
    return self();
  }
  Derived& pad_left(int32_t pad) {
    self().set_pad_left(pad);
    return self();
  }
  int32_t pad_left() const { return self().get_pad_left(); }

  Derived& pad_right(int32_t pad) {
    self().set_pad_right(pad);
    return self();
  }
  int32_t pad_right() const { return self().get_pad_right(); }

  Derived& pad_top(int32_t pad) {
    self().set_pad_top(pad);
    return self();
  }
  int32_t pad_top() const { return self().get_pad_top(); }

  Derived& pad_bottom(int32_t pad) {
    self().set_pad_bottom(pad);
    return self();
  }
  int32_t pad_bottom() const { return self().get_pad_bottom(); }

  Derived& pad_row(int32_t pad) {
    self().set_pad_row(pad);
    return self();
  }
  int32_t pad_row() const { return self().get_pad_row(); }

  Derived& pad_column(int32_t pad) {
    self().set_pad_column(pad);
    return self();
  }
  int32_t pad_column() const { return self().get_pad_column(); }
  Derived& pad_gap(int32_t gap) {
    self().set_pad_gap(gap);
    return self();
  }
  Derived& pad_radial(int32_t pad) {
    self().set_pad_radial(pad);
    return self();
  }

  // =========================================================================
  // Margin
  // =========================================================================

  Derived& margin_all(int32_t margin) {
    self().set_margin_all(margin);
    return self();
  }
  Derived& margin_hor(int32_t margin) {
    self().set_margin_hor(margin);
    return self();
  }
  Derived& margin_ver(int32_t margin) {
    self().set_margin_ver(margin);
    return self();
  }
  Derived& margin_left(int32_t margin) {
    self().set_margin_left(margin);
    return self();
  }
  int32_t margin_left() const { return self().get_margin_left(); }

  Derived& margin_right(int32_t margin) {
    self().set_margin_right(margin);
    return self();
  }
  int32_t margin_right() const { return self().get_margin_right(); }

  Derived& margin_top(int32_t margin) {
    self().set_margin_top(margin);
    return self();
  }
  int32_t margin_top() const { return self().get_margin_top(); }

  Derived& margin_bottom(int32_t margin) {
    self().set_margin_bottom(margin);
    return self();
  }
  int32_t margin_bottom() const { return self().get_margin_bottom(); }

  // =========================================================================
  // Size
  // =========================================================================

  Derived& width(int32_t value) {
    self().set_width(value);
    return self();
  }
  int32_t width() const { return self().get_width(); }

  Derived& min_width(int32_t value) {
    self().set_min_width(value);
    return self();
  }
  int32_t min_width() const { return self().get_min_width(); }

  Derived& max_width(int32_t value) {
    self().set_max_width(value);
    return self();
  }
  int32_t max_width() const { return self().get_max_width(); }

  Derived& height(int32_t value) {
    self().set_height(value);
    return self();
  }
  int32_t height() const { return self().get_height(); }

  Derived& min_height(int32_t value) {
    self().set_min_height(value);
    return self();
  }
  int32_t min_height() const { return self().get_min_height(); }

  Derived& max_height(int32_t value) {
    self().set_max_height(value);
    return self();
  }
  int32_t max_height() const { return self().get_max_height(); }
  Derived& x(int32_t value) {
    self().set_x(value);
    return self();
  }
  int32_t x() const { return self().get_x(); }
  Derived& y(int32_t value) {
    self().set_y(value);
    return self();
  }
  int32_t y() const { return self().get_y(); }
  Derived& size(int32_t width_val, int32_t height_val) {
    self().set_size(width_val, height_val);
    return self();
  }
  Derived& align(Align align_val) {
    self().set_align(static_cast<lv_align_t>(align_val));
    return self();
  }
  Derived& length(int32_t value) {
    self().set_length(value);
    return self();
  }
  int32_t length() const { return self().get_length(); }

  // =========================================================================
  // Geometry
  // =========================================================================

  Derived& radius(int32_t rad) {
    self().set_radius(rad);
    return self();
  }
  int32_t radius() const { return self().get_radius(); }
  Derived& clip_corner(bool clip) {
    self().set_clip_corner(clip);
    return self();
  }
  Derived& transform_width(int32_t value) {
    self().set_transform_width(value);
    return self();
  }
  Derived& transform_height(int32_t value) {
    self().set_transform_height(value);
    return self();
  }
  Derived& translate_x(int32_t value) {
    self().set_translate_x(value);
    return self();
  }
  int32_t translate_x() const { return self().get_translate_x(); }

  Derived& translate_y(int32_t value) {
    self().set_translate_y(value);
    return self();
  }
  int32_t translate_y() const { return self().get_translate_y(); }

  Derived& transform_scale_x(int32_t value) {
    self().set_transform_scale_x(value);
    return self();
  }
  int32_t transform_scale_x() const { return self().get_transform_scale_x(); }

  Derived& transform_scale_y(int32_t value) {
    self().set_transform_scale_y(value);
    return self();
  }
  int32_t transform_scale_y() const { return self().get_transform_scale_y(); }

  Derived& transform_scale(int32_t value) {
    self().set_transform_scale_x(value);
    self().set_transform_scale_y(value);
    return self();
  }

  Derived& transform_rotation(int32_t value) {
    self().set_transform_rotation(value);
    return self();
  }
  int32_t transform_rotation() const { return self().get_transform_rotation(); }
  Derived& transform_pivot_x(int32_t value) {
    self().set_transform_pivot_x(value);
    return self();
  }
  Derived& transform_pivot_y(int32_t value) {
    self().set_transform_pivot_y(value);
    return self();
  }
  Derived& transform_skew_x(int32_t value) {
    self().set_transform_skew_x(value);
    return self();
  }
  Derived& transform_skew_y(int32_t value) {
    self().set_transform_skew_y(value);
    return self();
  }
  Derived& translate_radial(int32_t value) {
    self().set_translate_radial(value);
    return self();
  }
  Derived& radial_offset(int32_t value) {
    self().set_radial_offset(value);
    return self();
  }

  // =========================================================================
  // Text
  // =========================================================================

  Derived& text_color(Color color) {
    self().set_text_color(color);
    return self();
  }
  Color text_color() const { return self().get_text_color(); }
  Derived& text_opa(Opacity opa) {
    self().set_text_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  Opacity text_opa() const {
    return static_cast<Opacity>(self().get_text_opa());
  }

  [[deprecated("Use text_opa(Opacity) instead")]]
  Derived& text_opa(lv_opa_t opa) {
    self().set_text_opa(opa);
    return self();
  }
  Derived& text_font(const lv_font_t* font) {
    self().set_text_font(font);
    return self();
  }
  Derived& text_font(const Font& font) {
    self().set_text_font(font.raw());
    return self();
  }
  Derived& text_recolor(bool recolor) {
    self().set_recolor(recolor);
    return self();
  }
  Derived& recolor(Color color) {
    self().set_recolor(color);
    return self();
  }
  Derived& recolor_opa(Opacity opa) {
    self().set_recolor_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  Derived& text_letter_space(int32_t space) {
    self().set_text_letter_space(space);
    return self();
  }
  int32_t text_letter_space() const { return self().get_text_letter_space(); }

  Derived& text_line_space(int32_t space) {
    self().set_text_line_space(space);
    return self();
  }
  int32_t text_line_space() const { return self().get_text_line_space(); }
  Derived& text_align(TextAlign align) {
    self().set_text_align(static_cast<lv_text_align_t>(align));
    return self();
  }
  [[deprecated("Use text_align(TextAlign) instead")]]
  Derived& text_align(lv_text_align_t align) {
    self().set_text_align(align);
    return self();
  }
  Derived& text_decor(TextDecor decor) {
    self().set_text_decor(static_cast<lv_text_decor_t>(decor));
    return self();
  }
  [[deprecated("Use text_decor(TextDecor) instead")]]
  Derived& text_decor(lv_text_decor_t decor) {
    self().set_text_decor(decor);
    return self();
  }
  Derived& text_outline_stroke_color(Color color) {
    self().set_text_outline_stroke_color(color);
    return self();
  }
  Derived& text_outline_stroke_opa(Opacity opa) {
    self().set_text_outline_stroke_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  Derived& text_outline_stroke_width(int32_t width) {
    self().set_text_outline_stroke_width(width);
    return self();
  }

  // =========================================================================
  // Image
  // =========================================================================

  Derived& image_opa(Opacity opa) {
    self().set_image_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  [[deprecated("Use image_opa(Opacity) instead")]]
  Derived& image_opa(lv_opa_t opa) {
    self().set_image_opa(opa);
    return self();
  }
  Derived& image_recolor(Color color) {
    self().set_image_recolor(color);
    return self();
  }
  Derived& image_recolor_opa(Opacity opa) {
    self().set_image_recolor_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  [[deprecated("Use image_recolor_opa(Opacity) instead")]]
  Derived& image_recolor_opa(lv_opa_t opa) {
    self().set_image_recolor_opa(opa);
    return self();
  }

  // =========================================================================
  // Line & Arc
  // =========================================================================

  Derived& line_width(int32_t width) {
    self().set_line_width(width);
    return self();
  }
  Derived& line_dash_width(int32_t width) {
    self().set_line_dash_width(width);
    return self();
  }
  Derived& line_dash_gap(int32_t gap) {
    self().set_line_dash_gap(gap);
    return self();
  }
  Derived& line_rounded(bool rounded) {
    self().set_line_rounded(rounded);
    return self();
  }
  Derived& line_color(Color color) {
    self().set_line_color(color);
    return self();
  }
  Derived& line_opa(Opacity opa) {
    self().set_line_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  [[deprecated("Use line_opa(Opacity) instead")]]
  Derived& line_opa(lv_opa_t opa) {
    self().set_line_opa(opa);
    return self();
  }

  Derived& arc_width(int32_t width) {
    self().set_arc_width(width);
    return self();
  }
  int32_t arc_width() const { return self().get_arc_width(); }

  Derived& arc_rounded(bool rounded) {
    self().set_arc_rounded(rounded);
    return self();
  }
  Derived& arc_color(Color color) {
    self().set_arc_color(color);
    return self();
  }
  Color arc_color() const { return self().get_arc_color(); }

  Derived& arc_opa(Opacity opa) {
    self().set_arc_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  Opacity arc_opa() const { return static_cast<Opacity>(self().get_arc_opa()); }
  [[deprecated("Use arc_opa(Opacity) instead")]]
  Derived& arc_opa(lv_opa_t opa) {
    self().set_arc_opa(opa);
    return self();
  }
  Derived& arc_image_src(const void* src) {
    self().set_arc_image_src(src);
    return self();
  }
  Derived& arc_image_src(const ImageDescriptor& dsc) {
    self().set_arc_image_src(dsc.raw());
    return self();
  }

  // =========================================================================
  // Animation
  // =========================================================================

  Derived& anim_time(uint32_t time) {
    self().set_anim_duration(time);
    return self();
  }
  Derived& anim_duration(uint32_t duration) {
    self().set_anim_duration(duration);
    return self();
  }
  Derived& transition(const lv_style_transition_dsc_t* dsc) {
    self().set_transition(dsc);
    return self();
  }

  // =========================================================================
  // Layout (Flex/Grid)
  // =========================================================================

  Derived& flex_flow(FlexFlow flow) {
    self().set_flex_flow(static_cast<lv_flex_flow_t>(flow));
    return self();
  }
  [[deprecated("Use flex_flow(FlexFlow) instead")]]
  Derived& flex_flow(lv_flex_flow_t flow) {
    self().set_flex_flow(flow);
    return self();
  }
  Derived& flex_main_place(FlexAlign place) {
    self().set_flex_main_place(static_cast<lv_flex_align_t>(place));
    return self();
  }
  [[deprecated("Use flex_main_place(FlexAlign) instead")]]
  Derived& flex_main_place(lv_flex_align_t place) {
    self().set_flex_main_place(place);
    return self();
  }
  Derived& flex_cross_place(FlexAlign place) {
    self().set_flex_cross_place(static_cast<lv_flex_align_t>(place));
    return self();
  }
  [[deprecated("Use flex_cross_place(FlexAlign) instead")]]
  Derived& flex_cross_place(lv_flex_align_t place) {
    self().set_flex_cross_place(place);
    return self();
  }
  Derived& flex_track_place(FlexAlign place) {
    self().set_flex_track_place(static_cast<lv_flex_align_t>(place));
    return self();
  }
  [[deprecated("Use flex_track_place(FlexAlign) instead")]]
  Derived& flex_track_place(lv_flex_align_t place) {
    self().set_flex_track_place(place);
    return self();
  }
  Derived& flex_grow(uint8_t grow) {
    self().set_flex_grow(grow);
    return self();
  }

  Derived& grid_row_dsc_array(const int32_t* dsc) {
    self().set_grid_row_dsc_array(dsc);
    return self();
  }
  Derived& grid_column_dsc_array(const int32_t* dsc) {
    self().set_grid_column_dsc_array(dsc);
    return self();
  }
  Derived& grid_row_align(GridAlign align) {
    self().set_grid_row_align(static_cast<lv_grid_align_t>(align));
    return self();
  }
  [[deprecated("Use grid_row_align(GridAlign) instead")]]
  Derived& grid_row_align(lv_grid_align_t align) {
    self().set_grid_row_align(align);
    return self();
  }
  Derived& grid_column_align(GridAlign align) {
    self().set_grid_column_align(static_cast<lv_grid_align_t>(align));
    return self();
  }
  [[deprecated("Use grid_column_align(GridAlign) instead")]]
  Derived& grid_column_align(lv_grid_align_t align) {
    self().set_grid_column_align(align);
    return self();
  }
  Derived& grid_cell_column_pos(int32_t pos) {
    self().set_grid_cell_column_pos(pos);
    return self();
  }
  Derived& grid_cell_column_span(int32_t span) {
    self().set_grid_cell_column_span(span);
    return self();
  }
  Derived& grid_cell_row_pos(int32_t pos) {
    self().set_grid_cell_row_pos(pos);
    return self();
  }
  Derived& grid_cell_row_span(int32_t span) {
    self().set_grid_cell_row_span(span);
    return self();
  }
  Derived& grid_cell_x_align(GridAlign align) {
    self().set_grid_cell_x_align(static_cast<lv_grid_align_t>(align));
    return self();
  }
  [[deprecated("Use grid_cell_x_align(GridAlign) instead")]]
  Derived& grid_cell_x_align(lv_grid_align_t align) {
    self().set_grid_cell_x_align(align);
    return self();
  }
  Derived& grid_cell_y_align(GridAlign align) {
    self().set_grid_cell_y_align(static_cast<lv_grid_align_t>(align));
    return self();
  }
  [[deprecated("Use grid_cell_y_align(GridAlign) instead")]]
  Derived& grid_cell_y_align(lv_grid_align_t align) {
    self().set_grid_cell_y_align(align);
    return self();
  }

  // =========================================================================
  // Miscellaneous
  // =========================================================================

  Derived& opa(Opacity opa) {
    self().set_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  Opacity opa() const { return static_cast<Opacity>(self().get_opa()); }

  [[deprecated("Use opa(Opacity) instead")]]
  Derived& opa(lv_opa_t opa) {
    self().set_opa(opa);
    return self();
  }
  Derived& blend_mode(BlendMode mode) {
    self().set_blend_mode(static_cast<lv_blend_mode_t>(mode));
    return self();
  }
  [[deprecated("Use blend_mode(BlendMode) instead")]]
  Derived& blend_mode(lv_blend_mode_t mode) {
    self().set_blend_mode(mode);
    return self();
  }
  Derived& base_dir(BaseDir dir) {
    self().set_base_dir(static_cast<lv_base_dir_t>(dir));
    return self();
  }
  [[deprecated("Use base_dir(BaseDir) instead")]]
  Derived& base_dir(lv_base_dir_t dir) {
    self().set_base_dir(dir);
    return self();
  }
  Derived& opa_layered(Opacity opa) {
    self().set_opa_layered(static_cast<lv_opa_t>(opa));
    return self();
  }
  Derived& rotary_sensitivity(uint32_t value) {
    self().set_rotary_sensitivity(value);
    return self();
  }
  Derived& color_filter_dsc(const lv_color_filter_dsc_t* dsc) {
    self().set_color_filter_dsc(dsc);
    return self();
  }
  Derived& color_filter_opa(Opacity opa) {
    self().set_color_filter_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
  Derived& anim(const lv_anim_t* anim) {
    self().set_anim(anim);
    return self();
  }

  Derived& layout(uint32_t value) {
    self().set_layout(value);
    return self();
  }
  uint32_t layout() const { return self().get_layout(); }
};

}  // namespace lvgl
