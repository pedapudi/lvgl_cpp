#ifndef LVGL_CPP_CORE_STYLE_H_
#define LVGL_CPP_CORE_STYLE_H_

#include <cstdint>

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

/**
 * @brief Wrapper for lv_style_t.
 */
class Style {
 public:
  Style();
  ~Style();

  // Non-copyable (styles are usually referenced by pointer, copying needs care)
  Style(const Style&) = delete;
  Style& operator=(const Style&) = delete;

  lv_style_t* raw() { return &style_; }
  const lv_style_t* raw() const { return &style_; }

  void set_bg_color(lv_color_t color);
  void set_bg_opa(lv_opa_t opa);
  void set_text_color(lv_color_t color);
  void set_pad_all(int32_t pad);

 private:
  lv_style_t style_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_STYLE_H_
