#pragma once

#include "../../font/font.h"
#include "../../misc/enums.h"
#include "../../misc/style.h"
#include "lvgl.h"

namespace lvgl {

template <typename Derived>
class Stylable {
 public:
  Derived& add_style(const lv_style_t* style,
                     lv_style_selector_t selector = 0) {
    lv_obj_add_style(static_cast<Derived*>(this)->raw(), style, selector);
    return *static_cast<Derived*>(this);
  }

  Derived& add_style(const Style& style, lv_style_selector_t selector = 0) {
    return add_style(style.raw(), selector);
  }

  Derived& add_style(const Style& style, Part part) {
    return add_style(style.raw(), static_cast<lv_style_selector_t>(part));
  }

  Derived& remove_style(const lv_style_t* style,
                        lv_style_selector_t selector = 0) {
    lv_obj_remove_style(static_cast<Derived*>(this)->raw(), style, selector);
    return *static_cast<Derived*>(this);
  }

  Derived& remove_style(const Style& style, lv_style_selector_t selector = 0) {
    return remove_style(style.raw(), selector);
  }

  Derived& remove_style(const Style& style, Part part) {
    return remove_style(style.raw(), static_cast<lv_style_selector_t>(part));
  }

  Derived& remove_all_styles() {
    lv_obj_remove_style_all(static_cast<Derived*>(this)->raw());
    return *static_cast<Derived*>(this);
  }

  // --- Fluent Style Setters ---

  // Background
  Derived& set_bg_color(lv_color_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_bg_color(static_cast<Derived*>(this)->raw(), value,
                              selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_bg_opa(lv_opa_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_bg_opa(static_cast<Derived*>(this)->raw(), value,
                            selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_bg_opa(Opacity value, lv_style_selector_t selector = 0) {
    return set_bg_opa(static_cast<lv_opa_t>(value), selector);
  }

  // Border
  Derived& set_border_width(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_border_width(static_cast<Derived*>(this)->raw(), value,
                                  selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_border_color(lv_color_t value,
                            lv_style_selector_t selector = 0) {
    lv_obj_set_style_border_color(static_cast<Derived*>(this)->raw(), value,
                                  selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_border_opa(lv_opa_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_border_opa(static_cast<Derived*>(this)->raw(), value,
                                selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_border_opa(Opacity value, lv_style_selector_t selector = 0) {
    return set_border_opa(static_cast<lv_opa_t>(value), selector);
  }

  Derived& set_border_side(lv_border_side_t value,
                           lv_style_selector_t selector = 0) {
    lv_obj_set_style_border_side(static_cast<Derived*>(this)->raw(), value,
                                 selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_border_side(BorderSide value, lv_style_selector_t selector = 0) {
    return set_border_side(static_cast<lv_border_side_t>(value), selector);
  }

  // Outline
  Derived& set_outline_width(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_outline_width(static_cast<Derived*>(this)->raw(), value,
                                   selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_outline_color(lv_color_t value,
                             lv_style_selector_t selector = 0) {
    lv_obj_set_style_outline_color(static_cast<Derived*>(this)->raw(), value,
                                   selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_outline_pad(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_outline_pad(static_cast<Derived*>(this)->raw(), value,
                                 selector);
    return *static_cast<Derived*>(this);
  }

  // Text
  Derived& set_text_color(lv_color_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_text_color(static_cast<Derived*>(this)->raw(), value,
                                selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_text_align(lv_text_align_t value,
                          lv_style_selector_t selector = 0) {
    lv_obj_set_style_text_align(static_cast<Derived*>(this)->raw(), value,
                                selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_text_align(TextAlign value, lv_style_selector_t selector = 0) {
    return set_text_align(static_cast<lv_text_align_t>(value), selector);
  }

  Derived& set_text_opa(lv_opa_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_text_opa(static_cast<Derived*>(this)->raw(), value,
                              selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_text_opa(Opacity value, lv_style_selector_t selector = 0) {
    return set_text_opa(static_cast<lv_opa_t>(value), selector);
  }

  Derived& set_text_font(const lv_font_t* value,
                         lv_style_selector_t selector = 0) {
    lv_obj_set_style_text_font(static_cast<Derived*>(this)->raw(), value,
                               selector);
    return *static_cast<Derived*>(this);
  }

  Derived& set_text_font(const Font& font, lv_style_selector_t selector = 0) {
    return set_text_font(font.raw(), selector);
  }

  // Image
  Derived& set_image_recolor(lv_color_t value,
                             lv_style_selector_t selector = 0) {
    lv_obj_set_style_image_recolor(static_cast<Derived*>(this)->raw(), value,
                                   selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_image_recolor_opa(lv_opa_t value,
                                 lv_style_selector_t selector = 0) {
    lv_obj_set_style_image_recolor_opa(static_cast<Derived*>(this)->raw(),
                                       value, selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_image_recolor_opa(Opacity value,
                                 lv_style_selector_t selector = 0) {
    return set_image_recolor_opa(static_cast<lv_opa_t>(value), selector);
  }

  Derived& set_style_blend_mode(BlendMode value,
                                lv_style_selector_t selector = 0) {
    lv_obj_set_style_blend_mode(static_cast<Derived*>(this)->raw(),
                                static_cast<lv_blend_mode_t>(value), selector);
    return *static_cast<Derived*>(this);
  }

  // Geometry / Padding
  Derived& set_radius(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_radius(static_cast<Derived*>(this)->raw(), value,
                            selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_pad_all(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_pad_all(static_cast<Derived*>(this)->raw(), value,
                             selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_pad_hor(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_pad_hor(static_cast<Derived*>(this)->raw(), value,
                             selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_pad_ver(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_pad_ver(static_cast<Derived*>(this)->raw(), value,
                             selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_pad_gap(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_pad_gap(static_cast<Derived*>(this)->raw(), value,
                             selector);
    return *static_cast<Derived*>(this);
  }

  // Explicit Style Size
  Derived& set_style_width(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_width(static_cast<Derived*>(this)->raw(), value, selector);
    return *static_cast<Derived*>(this);
  }
  Derived& set_style_height(int32_t value, lv_style_selector_t selector = 0) {
    lv_obj_set_style_height(static_cast<Derived*>(this)->raw(), value,
                            selector);
    return *static_cast<Derived*>(this);
  }
};

}  // namespace lvgl
