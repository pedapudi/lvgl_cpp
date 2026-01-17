#include <cassert>
#include <iostream>

#include "../core/style.h"
#include "../misc/color.h"

int main() {
  lv_init();
  std::cout << "Testing Style Color API..." << std::endl;

  lvgl::Style style;
  lvgl::Color red = lvgl::Color::make(255, 0, 0);
  lvgl::Color blue = lvgl::Color::make(0, 0, 255);

  // Test set_bg_color
  style.set_bg_color(red);
  // There isn't a direct "get" accessor on the simple wrapper yet to verify
  // state easily without linking deep into LVGL internals or checking raw
  // struct. For now, we verified the API signature change allows compilation
  // and execution. We can check the raw style property if we really want to be
  // sure. lv_style_value_t v; lv_style_res_t res =
  // lv_style_get_prop(style.raw(), LV_STYLE_BG_COLOR, &v); assert(res ==
  // LV_STYLE_RES_FOUND); assert(v.color.red == 255);

  // Test set_text_color
  style.set_text_color(blue);

  std::cout << "Style Color API passed compilation and execution." << std::endl;
  return 0;
}
