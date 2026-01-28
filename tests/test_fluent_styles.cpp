#include <cassert>
#include <iostream>

#include "../font/font.h"
#include "../widgets/button.h"
#include "lvgl.h"

int main() {
  lv_init();

  // Create a display (required for screens)
  lv_display_t* disp = lv_display_create(800, 600);
  // Use a minimal buffer if verify_draw logic is needed, but for style props it
  // might not be strict. However, lv_display_create typically expects a
  // flush_cb or setting buffers if using raw buffer. For linux frame buffer or
  // SDL, it sets up itself. In tests, we often assume the linux port or dummy.

  std::cout << "Testing Fluent Style Setters..." << std::endl;

  {
    lvgl::Button btn;
    btn.set_size(100, 50)
        .center()
        .style()
        .bg_color(lv_color_hex(0xFF0000))
        .bg_opa(lvgl::Opacity::Opa50)
        .border_width(5)
        .border_color(lv_color_hex(0x00FF00))
        .radius(10)
        .outline_width(2)
        .text_color(lv_color_hex(0x0000FF));

    // Verify properties
    lv_obj_t* obj = btn.raw();

    // Background color
    lv_color_t bg_color = lv_obj_get_style_bg_color(obj, LV_PART_MAIN);
    assert(lv_color_to_int(bg_color) ==
           lv_color_to_int(lv_color_hex(0xFF0000)));

    // Background opacity
    lv_opa_t bg_opa = lv_obj_get_style_bg_opa(obj, LV_PART_MAIN);
    assert(bg_opa == LV_OPA_50);

    // Border width
    int32_t border_width = lv_obj_get_style_border_width(obj, LV_PART_MAIN);
    assert(border_width == 5);

    // Border color
    lv_color_t border_color = lv_obj_get_style_border_color(obj, LV_PART_MAIN);
    assert(lv_color_to_int(border_color) ==
           lv_color_to_int(lv_color_hex(0x00FF00)));

    // Radius
    int32_t radius = lv_obj_get_style_radius(obj, LV_PART_MAIN);
    assert(radius == 10);

    // Outline width
    int32_t outline_width = lv_obj_get_style_outline_width(obj, LV_PART_MAIN);
    assert(outline_width == 2);

    // Text color
    lv_color_t text_color = lv_obj_get_style_text_color(obj, LV_PART_MAIN);
    assert(lv_color_to_int(text_color) ==
           lv_color_to_int(lv_color_hex(0x0000FF)));

    // Font (Standardization validaton)
    if (lvgl::Font::montserrat_14().is_valid()) {
      btn.style().text_font(lvgl::Font::montserrat_14());
      const lv_font_t* font = lv_obj_get_style_text_font(obj, LV_PART_MAIN);
      assert(font == lvgl::Font::montserrat_14().raw());
    }
  }

  std::cout << "[SUCCESS] Fluent style setters validated." << std::endl;
  return 0;
}
