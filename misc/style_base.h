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
  Derived& bg_grad_stop(int32_t value) {
    self().set_bg_grad_stop(value);
    return self();
  }
  Derived& bg_main_stop(int32_t value) {
    self().set_bg_main_stop(value);
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

  // =========================================================================
  // Border
  // =========================================================================

  Derived& border_color(Color color) {
    self().set_border_color(color);
    return self();
  }
  Derived& border_width(int32_t width) {
    self().set_border_width(width);
    return self();
  }
  Derived& border_opa(Opacity opa) {
    self().set_border_opa(static_cast<lv_opa_t>(opa));
    return self();
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
  Derived& pad_right(int32_t pad) {
    self().set_pad_right(pad);
    return self();
  }
  Derived& pad_top(int32_t pad) {
    self().set_pad_top(pad);
    return self();
  }
  Derived& pad_bottom(int32_t pad) {
    self().set_pad_bottom(pad);
    return self();
  }
  Derived& pad_row(int32_t pad) {
    self().set_pad_row(pad);
    return self();
  }
  Derived& pad_column(int32_t pad) {
    self().set_pad_column(pad);
    return self();
  }
  Derived& pad_gap(int32_t gap) {
    self().set_pad_gap(gap);
    return self();
  }

  // =========================================================================
  // Size
  // =========================================================================

  Derived& width(int32_t value) {
    self().set_width(value);
    return self();
  }
  Derived& min_width(int32_t value) {
    self().set_min_width(value);
    return self();
  }
  Derived& max_width(int32_t value) {
    self().set_max_width(value);
    return self();
  }
  Derived& height(int32_t value) {
    self().set_height(value);
    return self();
  }
  Derived& min_height(int32_t value) {
    self().set_min_height(value);
    return self();
  }
  Derived& max_height(int32_t value) {
    self().set_max_height(value);
    return self();
  }

  // =========================================================================
  // Geometry
  // =========================================================================

  Derived& radius(int32_t rad) {
    self().set_radius(rad);
    return self();
  }
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
  Derived& translate_y(int32_t value) {
    self().set_translate_y(value);
    return self();
  }
  Derived& transform_scale_x(int32_t value) {
    self().set_transform_scale_x(value);
    return self();
  }
  Derived& transform_scale_y(int32_t value) {
    self().set_transform_scale_y(value);
    return self();
  }
  Derived& transform_scale(int32_t value) {
    self().set_transform_scale_x(value);
    self().set_transform_scale_y(value);
    return self();
  }
  Derived& transform_rotation(int32_t value) {
    self().set_transform_rotation(value);
    return self();
  }
  Derived& transform_pivot_x(int32_t value) {
    self().set_transform_pivot_x(value);
    return self();
  }
  Derived& transform_pivot_y(int32_t value) {
    self().set_transform_pivot_y(value);
    return self();
  }

  // =========================================================================
  // Text
  // =========================================================================

  Derived& text_color(Color color) {
    self().set_text_color(color);
    return self();
  }
  Derived& text_opa(Opacity opa) {
    self().set_text_opa(static_cast<lv_opa_t>(opa));
    return self();
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
  Derived& text_letter_space(int32_t space) {
    self().set_text_letter_space(space);
    return self();
  }
  Derived& text_line_space(int32_t space) {
    self().set_text_line_space(space);
    return self();
  }
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
  Derived& arc_rounded(bool rounded) {
    self().set_arc_rounded(rounded);
    return self();
  }
  Derived& arc_color(Color color) {
    self().set_arc_color(color);
    return self();
  }
  Derived& arc_opa(Opacity opa) {
    self().set_arc_opa(static_cast<lv_opa_t>(opa));
    return self();
  }
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
};

}  // namespace lvgl
