#ifndef LVGL_CPP_MISC_STYLE_H_
#define LVGL_CPP_MISC_STYLE_H_

#include <cstdint>

#include "color.h"
#include "enums.h"
#include "lvgl.h"

namespace lvgl {

class Font;

/**
 * @brief Fluent wrapper for lv_style_t.
 *
 * The Style class provides a fluent API for defining LVGL styles. It owns
 * the underlying lv_style_t and automatically initializes it on construction.
 *
 * ## Usage
 *
 * ```cpp
 * auto style = lvgl::Style()
 *     .bg_color(lvgl::Color::from_hex(0xFF0000))
 *     .bg_opa(lvgl::Opacity::Cover)
 *     .radius(10)
 *     .border_width(2)
 *     .border_color(lvgl::Color::from_hex(0x0000FF))
 *     .pad_all(10);
 *
 * button.add_style(style, lvgl::Part::Main);
 * ```
 */
class Style {
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
  // Background
  // =========================================================================

  Style& bg_color(Color color);
  Style& bg_color(lv_color_t color);
  Style& bg_opa(Opacity opa);
  Style& bg_opa(lv_opa_t opa);
  Style& bg_grad_color(Color color);
  Style& bg_grad_dir(lv_grad_dir_t dir);

  // =========================================================================
  // Border
  // =========================================================================

  Style& border_color(Color color);
  Style& border_color(lv_color_t color);
  Style& border_width(int32_t width);
  Style& border_opa(Opacity opa);
  Style& border_opa(lv_opa_t opa);
  Style& border_side(BorderSide side);
  Style& border_side(lv_border_side_t side);

  // =========================================================================
  // Outline
  // =========================================================================

  Style& outline_color(Color color);
  Style& outline_color(lv_color_t color);
  Style& outline_width(int32_t width);
  Style& outline_opa(Opacity opa);
  Style& outline_opa(lv_opa_t opa);
  Style& outline_pad(int32_t pad);

  // =========================================================================
  // Padding
  // =========================================================================

  Style& pad_all(int32_t pad);
  Style& pad_hor(int32_t pad);
  Style& pad_ver(int32_t pad);
  Style& pad_left(int32_t pad);
  Style& pad_right(int32_t pad);
  Style& pad_top(int32_t pad);
  Style& pad_bottom(int32_t pad);
  Style& pad_gap(int32_t gap);
  Style& pad_row(int32_t pad);
  Style& pad_column(int32_t pad);

  // =========================================================================
  // Size
  // =========================================================================

  Style& width(int32_t value);
  Style& min_width(int32_t value);
  Style& max_width(int32_t value);
  Style& height(int32_t value);
  Style& min_height(int32_t value);
  Style& max_height(int32_t value);

  // =========================================================================
  // Geometry
  // =========================================================================

  Style& radius(int32_t radius);
  Style& clip_corner(bool clip);
  Style& transform_width(int32_t value);
  Style& transform_height(int32_t value);
  Style& translate_x(int32_t value);
  Style& translate_y(int32_t value);
  Style& transform_scale_x(int32_t value);
  Style& transform_scale_y(int32_t value);
  Style& transform_rotation(int32_t value);

  // =========================================================================
  // Text
  // =========================================================================

  Style& text_color(Color color);
  Style& text_color(lv_color_t color);
  Style& text_opa(Opacity opa);
  Style& text_opa(lv_opa_t opa);
  Style& text_font(const lv_font_t* font);
  Style& text_letter_space(int32_t space);
  Style& text_line_space(int32_t space);
  Style& text_align(TextAlign align);
  Style& text_align(lv_text_align_t align);
  Style& text_decor(TextDecor decor);
  Style& text_decor(lv_text_decor_t decor);

  // =========================================================================
  // Shadow
  // =========================================================================

  Style& shadow_color(Color color);
  Style& shadow_color(lv_color_t color);
  Style& shadow_opa(Opacity opa);
  Style& shadow_opa(lv_opa_t opa);
  Style& shadow_width(int32_t width);
  Style& shadow_offset_x(int32_t offset);
  Style& shadow_offset_y(int32_t offset);
  Style& shadow_spread(int32_t spread);

  // =========================================================================
  // Miscellaneous
  // =========================================================================

  Style& opa(Opacity opa);
  Style& opa(lv_opa_t opa);
  Style& blend_mode(BlendMode mode);
  Style& blend_mode(lv_blend_mode_t mode);

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
  lv_style_t style_;
  bool initialized_ = false;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_STYLE_H_
