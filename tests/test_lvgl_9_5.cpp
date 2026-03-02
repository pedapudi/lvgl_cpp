#include <cassert>
#include <iostream>

#include "../misc/style.h"
#include "../widgets/3dtexture.h"
#include "../widgets/arclabel.h"
#include "../widgets/chart.h"
#include "lvgl.h"

int main() {
  lv_init();

#if LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 5
  std::cout << "Testing LVGL 9.5 Features..." << std::endl;

  // Test ArcLabel
  lvgl::ArcLabel al(lv_screen_active());
  al.set_text("Test Arc").set_radius(50);
  assert(al.raw() != nullptr);
  std::cout << "ArcLabel OK" << std::endl;

  // Test 3DTexture
  lvgl::ThreeDTexture tex(lv_screen_active());
  assert(tex.raw() != nullptr);
  std::cout << "3DTexture OK" << std::endl;

  // Test Style Blur
  lvgl::Style style;
  style.blur_radius(10).blur_backdrop(true);
  std::cout << "Style Blur Properties OK" << std::endl;

  // Test Chart Curve Type
  lvgl::Chart chart(lv_screen_active());
  chart.set_type(lvgl::Chart::Type::Curve);
  assert(chart.get_type() == lvgl::Chart::Type::Curve);
  std::cout << "Chart Curve Type OK" << std::endl;

  std::cout << "All LVGL 9.5 Integration Tests Passed!" << std::endl;
#else
  std::cout << "LVGL 9.5 features not enabled/detected." << std::endl;
#endif

  return 0;
}
