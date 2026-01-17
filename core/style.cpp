#include "style.h"

namespace lvgl {

Style::Style() { lv_style_init(&style_); }

Style::~Style() { lv_style_reset(&style_); }

void Style::set_bg_color(Color color) { lv_style_set_bg_color(&style_, color); }

void Style::set_bg_opa(lv_opa_t opa) { lv_style_set_bg_opa(&style_, opa); }

void Style::set_text_color(Color color) {
  lv_style_set_text_color(&style_, color);
}

void Style::set_pad_all(int32_t pad) { lv_style_set_pad_all(&style_, pad); }

}  // namespace lvgl
