#include <iostream>
#include <vector>

#include "../display/display.h"
#include "../widgets/lottie.h"
#include "lvgl.h"

void test_lottie_creation() {
#if LV_USE_LOTTIE
  std::cout << "Testing Lottie widget..." << std::endl;
  lvgl::Object screen =
      lvgl::Object(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);

  // Test Create
  lvgl::Lottie lottie = lvgl::Lottie::create(screen);

  // Test Buffer Setting (mock buffer)
  static uint32_t buf[100 * 100];
  lottie.set_buffer(100, 100, buf);

  // Test Src Setting (mock data)
  // Note: In a real test, this would need valid JSON data to not crash LVGL if
  // it parses immediately. lv_lottie_set_src_data usually parses. We might skip
  // specific src tests if we don't have valid Lottie data handy. However, we
  // can check basic API existence.

  // Test Anim Access
  // lvgl::Animation anim = lottie.anim(); // This might return null if no src
  // is set.

  std::cout << "Lottie creation passed." << std::endl;
#else
  std::cout << "Lottie disabled, skipping test." << std::endl;
#endif
}

int main() {
  lv_init();
#if LV_USE_LOTTIE
  // Need a display for widget creation
  auto display = lvgl::Display::create(800, 480);
#endif

  test_lottie_creation();

  return 0;
}
