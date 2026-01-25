#pragma once

#include "../../misc/enums.h"
#include "../../misc/style.h"  // For color types etc
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Transient proxy object for setting local styles on a widget using a
 * fluent API.
 *
 * This class is returned by `Stylable::style()` and allows setting style
 * properties directly on the object (Local Style) using the same API as the
 * shared `Style` class.
 *
 * It is designed to be zero-overhead when compiled with optimization (all
 * methods inline).
 */
template <typename ObjType>
class StyleProxy {
 public:
  StyleProxy(ObjType* obj, lv_style_selector_t selector)
      : obj_(obj), selector_(selector) {}

  // =========================================================================
  // Background
  // =========================================================================

  StyleProxy& bg_color(Color color) {
    lv_obj_set_style_bg_color(obj_->raw(), color, selector_);
    return *this;
  }

  StyleProxy& bg_opa(Opacity opa) {
    lv_obj_set_style_bg_opa(obj_->raw(), static_cast<lv_opa_t>(opa), selector_);
    return *this;
  }

  // =========================================================================
  // Border
  // =========================================================================

  StyleProxy& border_width(int32_t width) {
    lv_obj_set_style_border_width(obj_->raw(), width, selector_);
    return *this;
  }

  StyleProxy& border_color(Color color) {
    lv_obj_set_style_border_color(obj_->raw(), color, selector_);
    return *this;
  }

  StyleProxy& border_opa(Opacity opa) {
    lv_obj_set_style_border_opa(obj_->raw(), static_cast<lv_opa_t>(opa),
                                selector_);
    return *this;
  }

  StyleProxy& border_side(BorderSide side) {
    lv_obj_set_style_border_side(
        obj_->raw(), static_cast<lv_border_side_t>(side), selector_);
    return *this;
  }

  // =========================================================================
  // Outline
  // =========================================================================

  StyleProxy& outline_width(int32_t width) {
    lv_obj_set_style_outline_width(obj_->raw(), width, selector_);
    return *this;
  }

  StyleProxy& outline_color(Color color) {
    lv_obj_set_style_outline_color(obj_->raw(), color, selector_);
    return *this;
  }

  StyleProxy& outline_pad(int32_t pad) {
    lv_obj_set_style_outline_pad(obj_->raw(), pad, selector_);
    return *this;
  }

  // =========================================================================
  // Geometry / Padding
  // =========================================================================

  StyleProxy& radius(int32_t value) {
    lv_obj_set_style_radius(obj_->raw(), value, selector_);
    return *this;
  }

  StyleProxy& pad_all(int32_t value) {
    lv_obj_set_style_pad_all(obj_->raw(), value, selector_);
    return *this;
  }

  StyleProxy& pad_hor(int32_t value) {
    lv_obj_set_style_pad_hor(obj_->raw(), value, selector_);
    return *this;
  }

  StyleProxy& pad_ver(int32_t value) {
    lv_obj_set_style_pad_ver(obj_->raw(), value, selector_);
    return *this;
  }

  StyleProxy& pad_gap(int32_t value) {
    lv_obj_set_style_pad_gap(obj_->raw(), value, selector_);
    return *this;
  }

  // =========================================================================
  // Size
  // =========================================================================

  StyleProxy& width(int32_t value) {
    lv_obj_set_style_width(obj_->raw(), value, selector_);
    return *this;
  }

  StyleProxy& height(int32_t value) {
    lv_obj_set_style_height(obj_->raw(), value, selector_);
    return *this;
  }

  // =========================================================================
  // Text
  // =========================================================================

  StyleProxy& text_color(Color color) {
    lv_obj_set_style_text_color(obj_->raw(), color, selector_);
    return *this;
  }

  StyleProxy& text_align(TextAlign align) {
    lv_obj_set_style_text_align(obj_->raw(),
                                static_cast<lv_text_align_t>(align), selector_);
    return *this;
  }

  StyleProxy& text_font(const Font& font) {
    lv_obj_set_style_text_font(obj_->raw(), font.raw(), selector_);
    return *this;
  }

 private:
  ObjType* obj_;
  lv_style_selector_t selector_;
};

}  // namespace lvgl
