#include <cassert>
#include <iostream>

#include "core/cast.h"
#include "display/display.h"
#include "../lvgl_cpp.h"
#include "widgets/button.h"
#include "widgets/label.h"

int main() {
  lv_init();
  auto disp = lvgl::Display::create(800, 600);

  std::cout << "Testing Safe Casting..." << std::endl;

  lvgl::Object screen(lv_screen_active());
  lvgl::Button btn(screen);
  lvgl::Label label(screen);

  // Test is<T>
  assert(lvgl::is<lvgl::Button>(btn) == true);
  assert(lvgl::is<lvgl::Object>(btn) == true);  // Should inherit
  assert(lvgl::is<lvgl::Label>(btn) == false);

  assert(lvgl::is<lvgl::Label>(label) == true);
  assert(lvgl::is<lvgl::Object>(label) == true);
  assert(lvgl::is<lvgl::Button>(label) == false);

  // Test is_exact<T>
  assert(lvgl::is_exact<lvgl::Button>(btn) == true);
  assert(lvgl::is_exact<lvgl::Object>(btn) ==
         false);  // Button is not EXACTLY an Object class

  // Test cast<T>
  {
    lvgl::Object obj_proxy(btn.raw(), lvgl::Object::Ownership::Unmanaged);
    auto casted_btn = lvgl::cast<lvgl::Button>(obj_proxy);
    assert(casted_btn.is_valid() == true);
    assert(casted_btn.raw() == btn.raw());

    auto failed_cast = lvgl::cast<lvgl::Label>(obj_proxy);
    assert(failed_cast.is_valid() == false);
  }

  std::cout << "[SUCCESS] Safe casting validated." << std::endl;
  return 0;
}
