#include <cassert>
#include <iostream>

#include "../widgets/screen.h"
#include "lvgl.h"

void test_screen_lifecycle() {
  std::cout << "Testing Screen Lifecycle..." << std::endl;

  // Create a new screen
  lvgl::Screen screen;
  assert(screen.is_valid());

  // Initially, it shouldn't be active (unless it's the very first, but let's
  // assume default screen exists) Note: Standard LVGL init creates a default
  // screen.

  // Load it
  screen.load();

  // Check active screen
  lvgl::Screen active_screen = lvgl::Screen::active();
  assert(active_screen.raw() == screen.raw());

  std::cout << "Screen Lifecycle Passed" << std::endl;
}

void test_global_layers() {
  std::cout << "Testing Global Layers..." << std::endl;

  lvgl::Screen top = lvgl::Screen::top();
  assert(top.is_valid());
  assert(top.raw() == lv_layer_top());

  lvgl::Screen sys = lvgl::Screen::sys();
  assert(sys.is_valid());
  assert(sys.raw() == lv_layer_sys());

  std::cout << "Global Layers Passed" << std::endl;
}

int main() {
  lv_init();

  // Need a display for screens to work
  lv_display_t* disp = lv_display_create(800, 480);

  test_screen_lifecycle();
  test_global_layers();

  return 0;
}
