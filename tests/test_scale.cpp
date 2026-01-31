#include <cassert>
#include <iostream>

#include "../display/display.h"
#include "../widgets/scale.h"
#include "lvgl.h"

void test_scale_enums() {
  std::cout << "Testing Scale Enums..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Scale scale(screen);

  scale.set_mode(lvgl::Scale::Mode::VerticalRight);
  assert(scale.get_mode() == lvgl::Scale::Mode::VerticalRight);
  assert(lv_scale_get_mode(scale.raw()) == LV_SCALE_MODE_VERTICAL_RIGHT);

  scale.set_mode(lvgl::Scale::Mode::RoundOuter);
  assert(scale.get_mode() == lvgl::Scale::Mode::RoundOuter);
  assert(lv_scale_get_mode(scale.raw()) == LV_SCALE_MODE_ROUND_OUTER);

  std::cout << "PASS: Scale enums verified." << std::endl;
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);
  test_scale_enums();
  std::cout << "All tests passed!" << std::endl;
  return 0;
}
