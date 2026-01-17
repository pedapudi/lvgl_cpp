#include <cassert>
#include <iostream>

#include "display/display.h"
#include "lvgl.h"

int main() {
  lv_init();

  // We can't easily create a real display without a driver in this test
  // environment, but the Display wrapper handles nullptr gracefully or we can
  // mock one if needed. However, the enum values are compile-time constants, so
  // we can verify those.

  static_assert(
      static_cast<int>(lvgl::Display::Rotation::ROT_0) == LV_DISPLAY_ROTATION_0,
      "ROT_0 mismatch");
  static_assert(static_cast<int>(lvgl::Display::Rotation::ROT_90) ==
                    LV_DISPLAY_ROTATION_90,
                "ROT_90 mismatch");
  static_assert(static_cast<int>(lvgl::Display::Rotation::ROT_180) ==
                    LV_DISPLAY_ROTATION_180,
                "ROT_180 mismatch");
  static_assert(static_cast<int>(lvgl::Display::Rotation::ROT_270) ==
                    LV_DISPLAY_ROTATION_270,
                "ROT_270 mismatch");

  std::cout << "Display::Rotation enum verification passed" << std::endl;
  return 0;
}
