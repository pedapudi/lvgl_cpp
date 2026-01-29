#include <cassert>
#include <iostream>
#include <vector>

#include "../core/object.h"
#include "../misc/animation.h"
#include "../misc/enums.h"
#include "lvgl.h"

// Mock for testing without full display driver if possible,
// otherwise this runs as a normal test in the suite.

void test_style_selector_operators() {
  std::cout << "Testing Style Selector Operators..." << std::endl;

  // Test Part | State
  lv_style_selector_t s1 = lvgl::Part::Indicator | lvgl::State::Checked;
  assert((s1 & LV_PART_INDICATOR) && (s1 & LV_STATE_CHECKED));

  // Test State | Part
  lv_style_selector_t s2 = lvgl::State::Pressed | lvgl::Part::Main;
  assert((s2 & LV_PART_MAIN) && (s2 & LV_STATE_PRESSED));

  // Test Part | Part (Masking)
  lvgl::Part p1 = lvgl::Part::Items | lvgl::Part::Selected;
  assert((static_cast<uint32_t>(p1) & LV_PART_ITEMS) &&
         (static_cast<uint32_t>(p1) & LV_PART_SELECTED));

  std::cout << "PASS: Style Selector Operators" << std::endl;
}

void test_style_proxy_fluent() {
  std::cout << "Testing Style Proxy Fluent API..." << std::endl;

  lvgl::Object obj(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);

  // Test chaining .part() and .state()
  obj.style()
      .part(lvgl::Part::Knob)
      .state(lvgl::State::Pressed)
      .bg_color(lvgl::Color::Red);

  // Verify it doesn't crash and compiles
  std::cout << "PASS: Style Proxy Fluent API" << std::endl;
}

void test_animation_typed_callback() {
  std::cout << "Testing Animation Typed Callback..." << std::endl;

  // Create a dummy object
  lvgl::Object obj(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  obj.set_x(0);

  // Create an animation
  lvgl::Animation anim(obj);
  anim.set_values(0, 100);
  anim.set_duration(100);

  // Set typed callback
  bool callback_called = false;
  anim.set_exec_cb([&callback_called](lvgl::Object& target, int32_t val) {
    callback_called = true;
    target.set_x(val);  // Should work on the wrapper
    assert(target.raw() != nullptr);
  });

  // Manually trigger the internal exec callback wrapper to verify it works
  // We can't easily wait for LVGL timer in a unit test without a main loop,
  // so we access the raw callback mechanisms or trust the wrapper logic.
  // However, since we modified Animation::set_exec_cb to wrap the lambda,
  // we can check if starting it sets the C callback.

  anim.start();

  // In a real integration test we'd let the loop run.
  // Here we can at least assert that compilation works and the setup didn't
  // crash.
  std::cout << "PASS: Animation Typed Callback setup (Runtime verification "
               "requires event loop)"
            << std::endl;
}

int main() {
  lv_init();

  // Need a display for screens?
  // lv_display_t * disp = lv_display_create(800, 480);
  // lvgl::Display display(disp);

  test_style_selector_operators();
  test_style_proxy_fluent();
  test_animation_typed_callback();

  return 0;
}
