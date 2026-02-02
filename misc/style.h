#ifndef LVGL_CPP_MISC_STYLE_H_
#define LVGL_CPP_MISC_STYLE_H_

#include "lvgl.h"
#include "style_base.h"

/**
 * @file style.h
 * @brief User Guide:
 * `Style` is a persistent container for UI properties. It can be shared among
 * multiple objects to maintain a consistent look and feel while saving memory.
 *
 * Key Features:
 * - **Fluent API**: Chained setters for all LVGL style properties.
 * - **Memory Efficient**: Internal `lv_style_t` management.
 * - **Type Safe**: Uses `lvgl_cpp` enums and classes (e.g., `Color`,
 * `Opacity`).
 *
 * Example:
 * `static Style my_style;`
 * `my_style.bg_color(Color::navy()).pad_all(10);`
 * `btn.add_style(my_style);`
 */

namespace lvgl {

class Font;

/**
 * @brief Persistent style object.
 *
 * Inherits the fluent API from StyleBase. Owns an lv_style_t.
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

 public:
  // Internal setter helpers (fluent)
  Style& set_bg_color(lv_color_t v) {
    lv_style_set_bg_color(&style_, v);
    return *this;
  }
  lv_color_t get_bg_color() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_BG_COLOR, &v) == LV_STYLE_RES_FOUND)
      return v.color;
    return lv_color_white();
  }
  Style& set_bg_opa(lv_opa_t v) {
    lv_style_set_bg_opa(&style_, v);
    return *this;
  }
  Style& set_bg_grad_color(lv_color_t v) {
    lv_style_set_bg_grad_color(&style_, v);
    return *this;
  }
  Style& set_bg_grad_dir(lv_grad_dir_t v) {
    lv_style_set_bg_grad_dir(&style_, v);
    return *this;
  }
  lv_grad_dir_t get_bg_grad_dir() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_BG_GRAD_DIR, &v) ==
        LV_STYLE_RES_FOUND)
      return (lv_grad_dir_t)v.num;
    return LV_GRAD_DIR_NONE;
  }
  Style& set_bg_grad_stop(int32_t v) {
    lv_style_set_bg_grad_stop(&style_, v);
    return *this;
  }
  Style& set_bg_main_stop(int32_t v) {
    lv_style_set_bg_main_stop(&style_, v);
    return *this;
  }
  Style& set_bg_grad(const lv_grad_dsc_t* v) {
    lv_style_set_bg_grad(&style_, v);
    return *this;
  }
  Style& set_bg_grad_opa(lv_opa_t v) {
    lv_style_set_bg_grad_opa(&style_, v);
    return *this;
  }
  Style& set_bg_main_opa(lv_opa_t v) {
    lv_style_set_bg_main_opa(&style_, v);
    return *this;
  }
  Style& set_bg_image_src(const void* v) {
    lv_style_set_bg_image_src(&style_, v);
    return *this;
  }
  Style& set_bg_image_opa(lv_opa_t v) {
    lv_style_set_bg_image_opa(&style_, v);
    return *this;
  }
  Style& set_bg_image_recolor(lv_color_t v) {
    lv_style_set_bg_image_recolor(&style_, v);
    return *this;
  }
  Style& set_bg_image_recolor_opa(lv_opa_t v) {
    lv_style_set_bg_image_recolor_opa(&style_, v);
    return *this;
  }
  Style& set_bg_image_tiled(bool v) {
    lv_style_set_bg_image_tiled(&style_, v);
    return *this;
  }
  Style& set_bitmap_mask_src(const void* v) {
    lv_style_set_bitmap_mask_src(&style_, v);
    return *this;
  }
  Style& set_image_colorkey(lv_color_t v) {
    lv_style_set_image_recolor(&style_, v);
    return *this;
  }

  Style& set_border_color(lv_color_t v) {
    lv_style_set_border_color(&style_, v);
    return *this;
  }
  lv_color_t get_border_color() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_BORDER_COLOR, &v) ==
        LV_STYLE_RES_FOUND)
      return v.color;
    return lv_color_black();
  }
  Style& set_border_width(int32_t v) {
    lv_style_set_border_width(&style_, v);
    return *this;
  }
  int32_t get_border_width() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_BORDER_WIDTH, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_border_opa(lv_opa_t v) {
    lv_style_set_border_opa(&style_, v);
    return *this;
  }
  lv_opa_t get_border_opa() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_BORDER_OPA, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return LV_OPA_COVER;
  }
  Style& set_border_side(lv_border_side_t v) {
    lv_style_set_border_side(&style_, v);
    return *this;
  }
  Style& set_border_post(bool v) {
    lv_style_set_border_post(&style_, v);
    return *this;
  }

  Style& set_outline_color(lv_color_t v) {
    lv_style_set_outline_color(&style_, v);
    return *this;
  }
  Style& set_outline_width(int32_t v) {
    lv_style_set_outline_width(&style_, v);
    return *this;
  }
  Style& set_outline_opa(lv_opa_t v) {
    lv_style_set_outline_opa(&style_, v);
    return *this;
  }
  Style& set_outline_pad(int32_t v) {
    lv_style_set_outline_pad(&style_, v);
    return *this;
  }

  Style& set_shadow_width(int32_t v) {
    lv_style_set_shadow_width(&style_, v);
    return *this;
  }
  Style& set_shadow_color(lv_color_t v) {
    lv_style_set_shadow_color(&style_, v);
    return *this;
  }
  Style& set_shadow_opa(lv_opa_t v) {
    lv_style_set_shadow_opa(&style_, v);
    return *this;
  }
  Style& set_shadow_offset_x(int32_t v) {
    lv_style_set_shadow_offset_x(&style_, v);
    return *this;
  }
  Style& set_shadow_offset_y(int32_t v) {
    lv_style_set_shadow_offset_y(&style_, v);
    return *this;
  }
  Style& set_shadow_spread(int32_t v) {
    lv_style_set_shadow_spread(&style_, v);
    return *this;
  }

  Style& set_pad_all(int32_t v) {
    lv_style_set_pad_all(&style_, v);
    return *this;
  }
  Style& set_pad_hor(int32_t v) {
    lv_style_set_pad_hor(&style_, v);
    return *this;
  }
  Style& set_pad_ver(int32_t v) {
    lv_style_set_pad_ver(&style_, v);
    return *this;
  }
  Style& set_pad_left(int32_t v) {
    lv_style_set_pad_left(&style_, v);
    return *this;
  }
  int32_t get_pad_left() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_PAD_LEFT, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_pad_right(int32_t v) {
    lv_style_set_pad_right(&style_, v);
    return *this;
  }
  int32_t get_pad_right() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_PAD_RIGHT, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_pad_top(int32_t v) {
    lv_style_set_pad_top(&style_, v);
    return *this;
  }
  int32_t get_pad_top() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_PAD_TOP, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_pad_bottom(int32_t v) {
    lv_style_set_pad_bottom(&style_, v);
    return *this;
  }
  int32_t get_pad_bottom() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_PAD_BOTTOM, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_pad_row(int32_t v) {
    lv_style_set_pad_row(&style_, v);
    return *this;
  }
  int32_t get_pad_row() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_PAD_ROW, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_pad_column(int32_t v) {
    lv_style_set_pad_column(&style_, v);
    return *this;
  }
  int32_t get_pad_column() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_PAD_COLUMN, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_pad_gap(int32_t v) {
    lv_style_set_pad_gap(&style_, v);
    return *this;
  }
  Style& set_pad_radial(int32_t v) {
    lv_style_set_pad_radial(&style_, v);
    return *this;
  }

  Style& set_margin_all(int32_t v) {
    lv_style_set_margin_all(&style_, v);
    return *this;
  }
  Style& set_margin_hor(int32_t v) {
    lv_style_set_margin_hor(&style_, v);
    return *this;
  }
  Style& set_margin_ver(int32_t v) {
    lv_style_set_margin_ver(&style_, v);
    return *this;
  }
  Style& set_margin_left(int32_t v) {
    lv_style_set_margin_left(&style_, v);
    return *this;
  }
  int32_t get_margin_left() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_MARGIN_LEFT, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_margin_right(int32_t v) {
    lv_style_set_margin_right(&style_, v);
    return *this;
  }
  int32_t get_margin_right() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_MARGIN_RIGHT, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_margin_top(int32_t v) {
    lv_style_set_margin_top(&style_, v);
    return *this;
  }
  int32_t get_margin_top() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_MARGIN_TOP, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_margin_bottom(int32_t v) {
    lv_style_set_margin_bottom(&style_, v);
    return *this;
  }
  int32_t get_margin_bottom() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_MARGIN_BOTTOM, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }

  Style& set_width(int32_t v) {
    lv_style_set_width(&style_, v);
    return *this;
  }
  int32_t get_width() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_WIDTH, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_min_width(int32_t v) {
    lv_style_set_min_width(&style_, v);
    return *this;
  }
  int32_t get_min_width() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_MIN_WIDTH, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_max_width(int32_t v) {
    lv_style_set_max_width(&style_, v);
    return *this;
  }
  int32_t get_max_width() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_MAX_WIDTH, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_height(int32_t v) {
    lv_style_set_height(&style_, v);
    return *this;
  }
  int32_t get_height() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_HEIGHT, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_min_height(int32_t v) {
    lv_style_set_min_height(&style_, v);
    return *this;
  }
  int32_t get_min_height() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_MIN_HEIGHT, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_max_height(int32_t v) {
    lv_style_set_max_height(&style_, v);
    return *this;
  }
  int32_t get_max_height() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_MAX_HEIGHT, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_x(int32_t v) {
    lv_style_set_x(&style_, v);
    return *this;
  }
  int32_t get_x() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_X, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_y(int32_t v) {
    lv_style_set_y(&style_, v);
    return *this;
  }
  int32_t get_y() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_Y, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_size(int32_t w, int32_t h) {
    lv_style_set_width(&style_, w);
    lv_style_set_height(&style_, h);
    return *this;
  }
  Style& set_align(lv_align_t v) {
    lv_style_set_align(&style_, v);
    return *this;
  }
  Style& set_length(int32_t v) {
    lv_style_set_length(&style_, v);
    return *this;
  }
  int32_t get_length() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_LENGTH, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }

  Style& set_radius(int32_t v) {
    lv_style_set_radius(&style_, v);
    return *this;
  }
  int32_t get_radius() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_RADIUS, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_clip_corner(bool v) {
    lv_style_set_clip_corner(&style_, v);
    return *this;
  }
  Style& set_transform_width(int32_t v) {
    lv_style_set_transform_width(&style_, v);
    return *this;
  }
  int32_t get_transform_width() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TRANSFORM_WIDTH, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_transform_height(int32_t v) {
    lv_style_set_transform_height(&style_, v);
    return *this;
  }
  int32_t get_transform_height() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TRANSFORM_HEIGHT, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_translate_x(int32_t v) {
    lv_style_set_translate_x(&style_, v);
    return *this;
  }
  int32_t get_translate_x() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TRANSLATE_X, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_translate_y(int32_t v) {
    lv_style_set_translate_y(&style_, v);
    return *this;
  }
  int32_t get_translate_y() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TRANSLATE_Y, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_transform_scale_x(int32_t v) {
    lv_style_set_transform_scale_x(&style_, v);
    return *this;
  }
  int32_t get_transform_scale_x() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TRANSFORM_SCALE_X, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 256;
  }
  Style& set_transform_scale_y(int32_t v) {
    lv_style_set_transform_scale_y(&style_, v);
    return *this;
  }
  int32_t get_transform_scale_y() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TRANSFORM_SCALE_Y, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 256;
  }
  Style& set_transform_rotation(int32_t v) {
    lv_style_set_transform_rotation(&style_, v);
    return *this;
  }
  int32_t get_transform_rotation() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TRANSFORM_ROTATION, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_transform_pivot_x(int32_t v) {
    lv_style_set_transform_pivot_x(&style_, v);
    return *this;
  }
  Style& set_transform_pivot_y(int32_t v) {
    lv_style_set_transform_pivot_y(&style_, v);
    return *this;
  }
  Style& set_transform_skew_x(int32_t v) {
    lv_style_set_transform_skew_x(&style_, v);
    return *this;
  }
  Style& set_transform_skew_y(int32_t v) {
    lv_style_set_transform_skew_y(&style_, v);
    return *this;
  }
  Style& set_translate_radial(int32_t v) {
    lv_style_set_translate_radial(&style_, v);
    return *this;
  }
  Style& set_radial_offset(int32_t v) {
    lv_style_set_radial_offset(&style_, v);
    return *this;
  }

  Style& set_text_color(lv_color_t v) {
    lv_style_set_text_color(&style_, v);
    return *this;
  }
  lv_color_t get_text_color() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TEXT_COLOR, &v) == LV_STYLE_RES_FOUND)
      return v.color;
    return lv_color_white();
  }
  Style& set_text_opa(lv_opa_t v) {
    lv_style_set_text_opa(&style_, v);
    return *this;
  }
  lv_opa_t get_text_opa() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TEXT_OPA, &v) == LV_STYLE_RES_FOUND)
      return (lv_opa_t)v.num;
    return LV_OPA_COVER;
  }
  Style& set_text_font(const lv_font_t* v) {
    lv_style_set_text_font(&style_, v);
    return *this;
  }
  Style& set_text_letter_space(int32_t v) {
    lv_style_set_text_letter_space(&style_, v);
    return *this;
  }
  int32_t get_text_letter_space() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TEXT_LETTER_SPACE, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_text_line_space(int32_t v) {
    lv_style_set_text_line_space(&style_, v);
    return *this;
  }
  int32_t get_text_line_space() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_TEXT_LINE_SPACE, &v) ==
        LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_text_align(lv_text_align_t v) {
    lv_style_set_text_align(&style_, v);
    return *this;
  }
  Style& set_text_decor(lv_text_decor_t v) {
    lv_style_set_text_decor(&style_, v);
    return *this;
  }
  Style& set_recolor(lv_color_t v) {
    lv_style_set_recolor(&style_, v);
    return *this;
  }
  Style& set_recolor_opa(lv_opa_t v) {
    lv_style_set_recolor_opa(&style_, v);
    return *this;
  }
  Style& set_text_outline_stroke_color(lv_color_t v) {
    lv_style_set_text_outline_stroke_color(&style_, v);
    return *this;
  }
  Style& set_text_outline_stroke_opa(lv_opa_t v) {
    lv_style_set_text_outline_stroke_opa(&style_, v);
    return *this;
  }
  Style& set_text_outline_stroke_width(int32_t v) {
    lv_style_set_text_outline_stroke_width(&style_, v);
    return *this;
  }

  Style& set_image_opa(lv_opa_t v) {
    lv_style_set_image_opa(&style_, v);
    return *this;
  }
  Style& set_image_recolor(lv_color_t v) {
    lv_style_set_image_recolor(&style_, v);
    return *this;
  }
  Style& set_image_recolor_opa(lv_opa_t v) {
    lv_style_set_image_recolor_opa(&style_, v);
    return *this;
  }

  Style& set_line_width(int32_t v) {
    lv_style_set_line_width(&style_, v);
    return *this;
  }
  Style& set_line_dash_width(int32_t v) {
    lv_style_set_line_dash_width(&style_, v);
    return *this;
  }
  Style& set_line_dash_gap(int32_t v) {
    lv_style_set_line_dash_gap(&style_, v);
    return *this;
  }
  Style& set_line_rounded(bool v) {
    lv_style_set_line_rounded(&style_, v);
    return *this;
  }
  Style& set_line_color(lv_color_t v) {
    lv_style_set_line_color(&style_, v);
    return *this;
  }
  Style& set_line_opa(lv_opa_t v) {
    lv_style_set_line_opa(&style_, v);
    return *this;
  }

  Style& set_arc_width(int32_t v) {
    lv_style_set_arc_width(&style_, v);
    return *this;
  }
  int32_t get_arc_width() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_ARC_WIDTH, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }
  Style& set_arc_rounded(bool v) {
    lv_style_set_arc_rounded(&style_, v);
    return *this;
  }
  Style& set_arc_color(lv_color_t v) {
    lv_style_set_arc_color(&style_, v);
    return *this;
  }
  lv_color_t get_arc_color() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_ARC_COLOR, &v) == LV_STYLE_RES_FOUND)
      return v.color;
    return lv_color_black();
  }
  Style& set_arc_opa(lv_opa_t v) {
    lv_style_set_arc_opa(&style_, v);
    return *this;
  }
  lv_opa_t get_arc_opa() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_ARC_OPA, &v) == LV_STYLE_RES_FOUND)
      return (lv_opa_t)v.num;
    return LV_OPA_COVER;
  }
  Style& set_arc_image_src(const void* v) {
    lv_style_set_arc_image_src(&style_, v);
    return *this;
  }

  Style& set_anim_duration(uint32_t v) {
    lv_style_set_anim_duration(&style_, v);
    return *this;
  }
  Style& set_transition(const lv_style_transition_dsc_t* v) {
    lv_style_set_transition(&style_, v);
    return *this;
  }

  Style& set_flex_flow(FlexFlow v) {
    lv_style_set_flex_flow(&style_, static_cast<lv_flex_flow_t>(v));
    return *this;
  }
  Style& set_flex_main_place(FlexAlign v) {
    lv_style_set_flex_main_place(&style_, static_cast<lv_flex_align_t>(v));
    return *this;
  }
  Style& set_flex_cross_place(FlexAlign v) {
    lv_style_set_flex_cross_place(&style_, static_cast<lv_flex_align_t>(v));
    return *this;
  }
  Style& set_flex_track_place(FlexAlign v) {
    lv_style_set_flex_track_place(&style_, static_cast<lv_flex_align_t>(v));
    return *this;
  }
  Style& set_flex_grow(uint32_t v) {
    lv_style_set_flex_grow(&style_, static_cast<uint32_t>(v));
    return *this;
  }

  Style& set_grid_row_dsc_array(const int32_t* v) {
    lv_style_set_grid_row_dsc_array(&style_, v);
    return *this;
  }
  Style& set_grid_column_dsc_array(const int32_t* v) {
    lv_style_set_grid_column_dsc_array(&style_, v);
    return *this;
  }
  Style& set_grid_row_align(GridAlign v) {
    lv_style_set_grid_row_align(&style_, static_cast<lv_grid_align_t>(v));
    return *this;
  }
  Style& set_grid_column_align(GridAlign v) {
    lv_style_set_grid_column_align(&style_, static_cast<lv_grid_align_t>(v));
    return *this;
  }
  Style& set_grid_cell_column_pos(int32_t v) {
    lv_style_set_grid_cell_column_pos(&style_, v);
    return *this;
  }
  Style& set_grid_cell_column_span(int32_t v) {
    lv_style_set_grid_cell_column_span(&style_, v);
    return *this;
  }
  Style& set_grid_cell_row_pos(int32_t v) {
    lv_style_set_grid_cell_row_pos(&style_, v);
    return *this;
  }
  Style& set_grid_cell_row_span(int32_t v) {
    lv_style_set_grid_cell_row_span(&style_, v);
    return *this;
  }
  Style& set_grid_cell_x_align(GridAlign v) {
    lv_style_set_grid_cell_x_align(&style_, static_cast<lv_grid_align_t>(v));
    return *this;
  }
  Style& set_grid_cell_y_align(GridAlign v) {
    lv_style_set_grid_cell_y_align(&style_, static_cast<lv_grid_align_t>(v));
    return *this;
  }

  Style& set_opa(lv_opa_t v) {
    lv_style_set_opa(&style_, v);
    return *this;
  }
  lv_opa_t get_opa() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_OPA, &v) == LV_STYLE_RES_FOUND)
      return (lv_opa_t)v.num;
    return LV_OPA_COVER;
  }
  Style& set_blend_mode(lv_blend_mode_t v) {
    lv_style_set_blend_mode(&style_, v);
    return *this;
  }
  Style& set_base_dir(lv_base_dir_t v) {
    lv_style_set_base_dir(&style_, v);
    return *this;
  }
  Style& set_color_filter_dsc(const lv_color_filter_dsc_t* v) {
    lv_style_set_color_filter_dsc(&style_, v);
    return *this;
  }
  Style& set_color_filter_opa(lv_opa_t v) {
    lv_style_set_color_filter_opa(&style_, v);
    return *this;
  }
  Style& set_anim(const lv_anim_t* v) {
    lv_style_set_anim(&style_, v);
    return *this;
  }
  Style& set_opa_layered(lv_opa_t v) {
    lv_style_set_opa_layered(&style_, v);
    return *this;
  }
  Style& set_rotary_sensitivity(uint32_t v) {
    lv_style_set_rotary_sensitivity(&style_, v);
    return *this;
  }

  Style& set_layout(uint32_t v) {
    lv_style_set_layout(&style_, v);
    return *this;
  }
  uint32_t get_layout() const {
    lv_style_value_t v;
    if (lv_style_get_prop(raw(), LV_STYLE_LAYOUT, &v) == LV_STYLE_RES_FOUND)
      return v.num;
    return 0;
  }

  lv_style_t style_;
  bool initialized_ = false;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_STYLE_H_
