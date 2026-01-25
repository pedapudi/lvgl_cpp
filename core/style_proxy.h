#pragma once

#include "../../font/font.h"
#include "../../misc/enums.h"
#include "../../misc/style.h"  // For color types etc
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Transient proxy object for setting local styles on a widget using a
 * fluent API.
 *
 * This class is returned by `Object::style()` and allows setting style
 * properties directly on the object (Local Style) using a fluent API.
 */
class StyleProxy {
 public:
  StyleProxy(lv_obj_t* obj, lv_style_selector_t selector)
      : obj_(obj), selector_(selector) {}

  // =========================================================================
  // Background
  // =========================================================================

  StyleProxy& bg_color(Color color) {
    lv_obj_set_style_bg_color(obj_, color, selector_);
    return *this;
  }

  StyleProxy& bg_opa(Opacity opa) {
    lv_obj_set_style_bg_opa(obj_, static_cast<lv_opa_t>(opa), selector_);
    return *this;
  }

  StyleProxy& bg_image_src(const void* src) {
    lv_obj_set_style_bg_image_src(obj_, src, selector_);
    return *this;
  }

  StyleProxy& bg_grad_color(Color color) {
    lv_obj_set_style_bg_grad_color(obj_, color, selector_);
    return *this;
  }

  StyleProxy& bg_grad_dir(lv_grad_dir_t value) {
    lv_obj_set_style_bg_grad_dir(obj_, value, selector_);
    return *this;
  }

  // =========================================================================
  // Border
  // =========================================================================

  StyleProxy& border_width(int32_t width) {
    lv_obj_set_style_border_width(obj_, width, selector_);
    return *this;
  }

  StyleProxy& border_color(Color color) {
    lv_obj_set_style_border_color(obj_, color, selector_);
    return *this;
  }

  StyleProxy& border_opa(Opacity opa) {
    lv_obj_set_style_border_opa(obj_, static_cast<lv_opa_t>(opa), selector_);
    return *this;
  }

  StyleProxy& border_side(BorderSide side) {
    lv_obj_set_style_border_side(obj_, static_cast<lv_border_side_t>(side),
                                 selector_);
    return *this;
  }

  // =========================================================================
  // Outline
  // =========================================================================

  StyleProxy& outline_width(int32_t width) {
    lv_obj_set_style_outline_width(obj_, width, selector_);
    return *this;
  }

  StyleProxy& outline_color(Color color) {
    lv_obj_set_style_outline_color(obj_, color, selector_);
    return *this;
  }

  StyleProxy& outline_pad(int32_t pad) {
    lv_obj_set_style_outline_pad(obj_, pad, selector_);
    return *this;
  }

  StyleProxy& outline_opa(Opacity opa) {
    lv_obj_set_style_outline_opa(obj_, static_cast<lv_opa_t>(opa), selector_);
    return *this;
  }

  // =========================================================================
  // Shadow
  // =========================================================================

  StyleProxy& shadow_width(int32_t width) {
    lv_obj_set_style_shadow_width(obj_, width, selector_);
    return *this;
  }

  StyleProxy& shadow_color(Color color) {
    lv_obj_set_style_shadow_color(obj_, color, selector_);
    return *this;
  }

  StyleProxy& shadow_opa(Opacity opa) {
    lv_obj_set_style_shadow_opa(obj_, static_cast<lv_opa_t>(opa), selector_);
    return *this;
  }

  StyleProxy& shadow_ofs_x(int32_t value) {
    lv_obj_set_style_shadow_ofs_x(obj_, value, selector_);
    return *this;
  }

  StyleProxy& shadow_ofs_y(int32_t value) {
    lv_obj_set_style_shadow_ofs_y(obj_, value, selector_);
    return *this;
  }

  StyleProxy& shadow_spread(int32_t value) {
    lv_obj_set_style_shadow_spread(obj_, value, selector_);
    return *this;
  }

  // =========================================================================
  // Geometry / Padding
  // =========================================================================

  StyleProxy& radius(int32_t value) {
    lv_obj_set_style_radius(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_all(int32_t value) {
    lv_obj_set_style_pad_all(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_hor(int32_t value) {
    lv_obj_set_style_pad_hor(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_ver(int32_t value) {
    lv_obj_set_style_pad_ver(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_left(int32_t value) {
    lv_obj_set_style_pad_left(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_right(int32_t value) {
    lv_obj_set_style_pad_right(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_top(int32_t value) {
    lv_obj_set_style_pad_top(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_bottom(int32_t value) {
    lv_obj_set_style_pad_bottom(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_row(int32_t value) {
    lv_obj_set_style_pad_row(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_column(int32_t value) {
    lv_obj_set_style_pad_column(obj_, value, selector_);
    return *this;
  }

  StyleProxy& pad_gap(int32_t value) {
    lv_obj_set_style_pad_gap(obj_, value, selector_);
    return *this;
  }

  // =========================================================================
  // Size
  // =========================================================================

  StyleProxy& width(int32_t value) {
    lv_obj_set_style_width(obj_, value, selector_);
    return *this;
  }

  StyleProxy& height(int32_t value) {
    lv_obj_set_style_height(obj_, value, selector_);
    return *this;
  }

  StyleProxy& min_width(int32_t value) {
    lv_obj_set_style_min_width(obj_, value, selector_);
    return *this;
  }

  StyleProxy& max_width(int32_t value) {
    lv_obj_set_style_max_width(obj_, value, selector_);
    return *this;
  }

  StyleProxy& min_height(int32_t value) {
    lv_obj_set_style_min_height(obj_, value, selector_);
    return *this;
  }

  StyleProxy& max_height(int32_t value) {
    lv_obj_set_style_max_height(obj_, value, selector_);
    return *this;
  }

  // =========================================================================
  // Text
  // =========================================================================

  StyleProxy& text_color(Color color) {
    lv_obj_set_style_text_color(obj_, color, selector_);
    return *this;
  }

  StyleProxy& text_opa(Opacity opa) {
    lv_obj_set_style_text_opa(obj_, static_cast<lv_opa_t>(opa), selector_);
    return *this;
  }

  StyleProxy& text_align(TextAlign align) {
    lv_obj_set_style_text_align(obj_, static_cast<lv_text_align_t>(align),
                                selector_);
    return *this;
  }

  StyleProxy& text_font(const Font& font) {
    lv_obj_set_style_text_font(obj_, font.raw(), selector_);
    return *this;
  }

  StyleProxy& text_letter_space(int32_t value) {
    lv_obj_set_style_text_letter_space(obj_, value, selector_);
    return *this;
  }

  StyleProxy& text_line_space(int32_t value) {
    lv_obj_set_style_text_line_space(obj_, value, selector_);
    return *this;
  }

  StyleProxy& text_decor(lv_text_decor_t value) {
    lv_obj_set_style_text_decor(obj_, value, selector_);
    return *this;
  }

  // =========================================================================
  // Image
  // =========================================================================

  StyleProxy& image_recolor(Color color) {
    lv_obj_set_style_image_recolor(obj_, color, selector_);
    return *this;
  }

  StyleProxy& image_recolor_opa(Opacity opa) {
    lv_obj_set_style_image_recolor_opa(obj_, static_cast<lv_opa_t>(opa),
                                       selector_);
    return *this;
  }

  StyleProxy& image_opa(Opacity opa) {
    lv_obj_set_style_image_opa(obj_, static_cast<lv_opa_t>(opa), selector_);
    return *this;
  }

  // =========================================================================
  // Line & Arc
  // =========================================================================

  StyleProxy& line_width(int32_t width) {
    lv_obj_set_style_line_width(obj_, width, selector_);
    return *this;
  }

  StyleProxy& line_dash_width(int32_t width) {
    lv_obj_set_style_line_dash_width(obj_, width, selector_);
    return *this;
  }

  StyleProxy& line_dash_gap(int32_t value) {
    lv_obj_set_style_line_dash_gap(obj_, value, selector_);
    return *this;
  }

  StyleProxy& line_rounded(bool value) {
    lv_obj_set_style_line_rounded(obj_, value, selector_);
    return *this;
  }

  StyleProxy& line_color(Color color) {
    lv_obj_set_style_line_color(obj_, color, selector_);
    return *this;
  }

  StyleProxy& arc_width(int32_t width) {
    lv_obj_set_style_arc_width(obj_, width, selector_);
    return *this;
  }

  StyleProxy& arc_rounded(bool value) {
    lv_obj_set_style_arc_rounded(obj_, value, selector_);
    return *this;
  }

  StyleProxy& arc_color(Color color) {
    lv_obj_set_style_arc_color(obj_, color, selector_);
    return *this;
  }

  // =========================================================================
  // Misc
  // =========================================================================

  StyleProxy& opa(Opacity opa) {
    lv_obj_set_style_opa(obj_, static_cast<lv_opa_t>(opa), selector_);
    return *this;
  }

  StyleProxy& blend_mode(BlendMode mode) {
    lv_obj_set_style_blend_mode(obj_, static_cast<lv_blend_mode_t>(mode),
                                selector_);
    return *this;
  }

  StyleProxy& anim_time(uint32_t value) {
    lv_obj_set_style_anim_duration(obj_, value, selector_);
    return *this;
  }

  StyleProxy& transform_width(int32_t value) {
    lv_obj_set_style_transform_width(obj_, value, selector_);
    return *this;
  }

  StyleProxy& transform_height(int32_t value) {
    lv_obj_set_style_transform_height(obj_, value, selector_);
    return *this;
  }

  StyleProxy& translate_x(int32_t value) {
    lv_obj_set_style_translate_x(obj_, value, selector_);
    return *this;
  }

  StyleProxy& translate_y(int32_t value) {
    lv_obj_set_style_translate_y(obj_, value, selector_);
    return *this;
  }

  /**
   * @brief Enable/disable clip corner (overflow: hidden).
   */
  StyleProxy& clip_corner(bool value) {
    lv_obj_set_style_clip_corner(obj_, value, selector_);
    return *this;
  }

  // =========================================================================
  // Transform & Transitions
  // =========================================================================

  /**
   * @brief Set the rotation of the object.
   * @param value Rotation in 0.1 degree units (e.g. 900 for 90 degrees).
   */
  StyleProxy& transform_rotation(int32_t value) {
    lv_obj_set_style_transform_rotation(obj_, value, selector_);
    return *this;
  }

  StyleProxy& transform_scale_x(int32_t value) {
    lv_obj_set_style_transform_scale_x(obj_, value, selector_);
    return *this;
  }

  StyleProxy& transform_scale_y(int32_t value) {
    lv_obj_set_style_transform_scale_y(obj_, value, selector_);
    return *this;
  }

  StyleProxy& transform_scale(int32_t value) {
    lv_obj_set_style_transform_scale_x(obj_, value, selector_);
    lv_obj_set_style_transform_scale_y(obj_, value, selector_);
    return *this;
  }

  StyleProxy& transform_pivot_x(int32_t value) {
    lv_obj_set_style_transform_pivot_x(obj_, value, selector_);
    return *this;
  }

  StyleProxy& transform_pivot_y(int32_t value) {
    lv_obj_set_style_transform_pivot_y(obj_, value, selector_);
    return *this;
  }

  StyleProxy& transition(const lv_style_transition_dsc_t* value) {
    lv_obj_set_style_transition(obj_, value, selector_);
    return *this;
  }

 private:
  lv_obj_t* obj_;
  lv_style_selector_t selector_;
};

}  // namespace lvgl
