#include <cassert>
#include <iostream>

#include "../display/display.h"
#include "../misc/style.h"
#include "../widgets/button.h"
#include "lvgl.h"

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  std::cout << "Testing Style Reconciliation..." << std::endl;

  // 1. Test Persistent Style
  lvgl::Style style;
  style.bg_color(lvgl::Color::from_hex(0xFF0000))
      .radius(10)
      .border_width(2)
      .shadow_ofs_x(5)
      .pad_all(15);

  lv_style_value_t v;
  lv_style_get_prop(style.raw(), LV_STYLE_BG_COLOR, &v);
  assert(lv_color_to_int(v.color) == 0xFF0000);
  lv_style_get_prop(style.raw(), LV_STYLE_RADIUS, &v);
  assert(v.num == 10);
  lv_style_get_prop(style.raw(), LV_STYLE_SHADOW_OFFSET_X, &v);
  assert(v.num == 5);

  // 2. Test Local Style (StyleProxy)
  lvgl::Button btn;
  btn.style()
      .bg_color(lvgl::Color::from_hex(0xFF0000))
      .radius(10)
      .border_width(2)
      .shadow_ofs_x(5)
      .pad_all(15);

  assert(lv_color_to_int(lv_obj_get_style_bg_color(btn.raw(), LV_PART_MAIN)) ==
         0xFF0000);
  assert(lv_obj_get_style_radius(btn.raw(), LV_PART_MAIN) == 10);
  assert(lv_obj_get_style_shadow_offset_x(btn.raw(), LV_PART_MAIN) == 5);

  std::cout << "[SUCCESS] Style and StyleProxy API unified and verified."
            << std::endl;
  return 0;
}
