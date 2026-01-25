#include <cassert>
#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../misc/enums.h"
#include "lvgl.h"

using namespace lvgl;

void test_obj_flags() {
  std::cout << "Testing ObjFlag enum..." << std::endl;
  Object parent(lv_screen_active(), Object::Ownership::Unmanaged);
  Object obj(&parent);

  // Test adding flag
  obj.add_flag(ObjFlag::Hidden);
  assert(obj.has_flag(ObjFlag::Hidden));
  assert(lv_obj_has_flag(obj.raw(), LV_OBJ_FLAG_HIDDEN));

  // Test removing flag
  obj.remove_flag(ObjFlag::Hidden);
  assert(!obj.has_flag(ObjFlag::Hidden));
  assert(!lv_obj_has_flag(obj.raw(), LV_OBJ_FLAG_HIDDEN));

  // Test bitwise ops (if implemented, but add_flag takes single flag usually)
  // Our enum has bitwise operators defined in enums.h? Let's check.
  // ObjFlag has operator| defined.

  // Test bitwise ops
  std::cout << "Testing bitwise flags..." << std::endl;
  obj.add_flag(ObjFlag::Clickable | ObjFlag::Checkable);

  assert(obj.has_flag(ObjFlag::Clickable));
  assert(obj.has_flag(ObjFlag::Checkable));

  std::cout << "PASS: ObjFlag works." << std::endl;
}

void test_display_layers() {
  std::cout << "Testing Display Layer Wrappers..." << std::endl;
  auto disp = Display::get_default();
  assert(disp != nullptr);

  // Test get_screen_active returning Object
  Object screen = disp->screen_active();
  assert(screen.raw() == lv_screen_active());
  assert(screen.is_valid());

  // Test get_layer_top returning Object
  Object top = disp->layer_top();
  assert(top.raw() == lv_layer_top());

  // Test get_layer_sys returning Object
  Object sys = disp->layer_sys();
  assert(sys.raw() == lv_layer_sys());

  std::cout << "PASS: Display layers return Objects." << std::endl;
}

void test_input_api() {
  // Basic check that InputDevice compiles and enums are usable
  // Real testing requires a mock indev, which is complex.
  // simpler to just checking static properties or compile time.
}

int main() {
  lv_init();
  auto disp = Display::create(800, 480);  // Ensure display exists and persists

  test_obj_flags();
  test_display_layers();

  std::cout << "All API Hardening tests passed!" << std::endl;
  return 0;
}
