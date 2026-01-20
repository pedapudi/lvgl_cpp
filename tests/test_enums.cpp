#include <cassert>
#include <iostream>

#include "../display/display.h"
#include "../widgets/button.h"
#include "../widgets/label.h"
#include "lvgl.h"

int main() {
  lv_init();

  // Create a display (required for screens)
  auto disp = lvgl::Display::create(800, 600);

  std::cout << "Testing Scoped Enums..." << std::endl;

  {
    lvgl::Button btn;
    btn.set_size(100, 50)
        .set_bg_opa(lvgl::Opacity::Cover)
        .set_border_opa(lvgl::Opacity::Opa50)
        .set_border_side(lvgl::BorderSide::Bottom);

    // Verify
    lv_obj_t* obj = btn.raw();
    assert(lv_obj_get_style_bg_opa(obj, LV_PART_MAIN) == LV_OPA_COVER);
    assert(lv_obj_get_style_border_opa(obj, LV_PART_MAIN) == LV_OPA_50);
    assert(lv_obj_get_style_border_side(obj, LV_PART_MAIN) ==
           LV_BORDER_SIDE_BOTTOM);
  }

  {
    lvgl::Label label;
    label.set_text("Hello");
    label.set_text_align(lvgl::TextAlign::Center);
    label.set_text_opa(lvgl::Opacity::Opa80);

    // Verify
    lv_obj_t* obj = label.raw();
    assert(lv_obj_get_style_text_align(obj, LV_PART_MAIN) ==
           LV_TEXT_ALIGN_CENTER);
    assert(lv_obj_get_style_text_opa(obj, LV_PART_MAIN) == LV_OPA_80);
  }

  std::cout << "[SUCCESS] Scoped Enums validated." << std::endl;
  return 0;
}
