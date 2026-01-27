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

  // Test Buffer Setting with DrawBuf
  lvgl::DrawBuf db(100, 100);
  lottie.set_draw_buf(db);

  // Test Auto-Buffer Create
  lvgl::Lottie lottie2 = lvgl::Lottie::create(screen, 128, 128);

  if (lottie2.raw()) {
    std::cout << "Lottie auto-buffer creation passed." << std::endl;
  }

  std::cout << "Lottie basic tests passed." << std::endl;
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
