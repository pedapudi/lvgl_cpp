#include <cassert>
#include <iostream>

#include "../widgets/button.h"
#include "../widgets/dropdown.h"
#include "../widgets/roller.h"
#include "../widgets/slider.h"
#include "../widgets/switch.h"
#include "lvgl.h"

void test_generic_callbacks() {
  std::cout << "Testing Generic Callbacks..." << std::endl;
  lvgl::Button btn;
  bool clicked = false;
  bool pressed = false;

  // Fluent chaining check
  btn.on_clicked([&](lvgl::Event& e) { clicked = true; })
      .on_pressed([&](lvgl::Event& e) { pressed = true; })
      .set_size(100, 50);  // Checks return type covariance

  // Trigger events
  lv_obj_send_event(btn.raw(), LV_EVENT_CLICKED, nullptr);
  assert(clicked);

  lv_obj_send_event(btn.raw(), LV_EVENT_PRESSED, nullptr);
  assert(pressed);

  std::cout << "Generic Callbacks Passed" << std::endl;
}

void test_slider_change() {
  std::cout << "Testing Slider Change..." << std::endl;
  lvgl::Slider slider;
  int value = -1;

  slider.on_value_changed([&](lvgl::Event& e) { value = slider.get_value(); });

  // Changing value programmatically usually doesn't fire event in LVGL
  // unless LV_ANIM_ON or manually sent.
  // Let's send event manually to verify callback registration.
  slider.set_value(50, LV_ANIM_OFF);
  lv_obj_send_event(slider.raw(), LV_EVENT_VALUE_CHANGED, nullptr);

  assert(value == 50);
  std::cout << "Slider Change Passed" << std::endl;
}

void test_switch_change() {
  std::cout << "Testing Switch Change..." << std::endl;
  lvgl::Switch sw;
  bool toggled = false;

  sw.on_value_changed([&](lvgl::Event& e) { toggled = true; });

  lv_obj_send_event(sw.raw(), LV_EVENT_VALUE_CHANGED, nullptr);
  assert(toggled);
  std::cout << "Switch Change Passed" << std::endl;
}

int main() {
  lv_init();
  lv_display_create(
      800,
      480);  // Requirement for creating widgets? Usually not if parent is null.
  // Screen creation happens implicitly or explicitly.
  // Since we use default constructor, it might use active screen.
  // Ensure display exists.

  test_generic_callbacks();
  test_slider_change();
  test_switch_change();

  return 0;
}
