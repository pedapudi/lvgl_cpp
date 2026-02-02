#include <cassert>
#include <iostream>

#include "../widgets/arc.h"
#include "../widgets/button.h"
#include "../widgets/calendar.h"
#include "../widgets/checkbox.h"
#include "../widgets/dropdown.h"
#include "../widgets/roller.h"
#include "../widgets/slider.h"
#include "../widgets/spinbox.h"
#include "../widgets/switch.h"
#include "../widgets/table.h"
#include "../widgets/textarea.h"
#include "../lvgl_cpp.h"

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
  (btn).send_event(lvgl::EventCode::Clicked);
  assert(clicked);

  (btn).send_event(lvgl::EventCode::Pressed);
  assert(pressed);

  std::cout << "Generic Callbacks Passed" << std::endl;
}

void test_slider_change() {
  std::cout << "Testing Slider Change..." << std::endl;
  lvgl::Slider slider;
  int value = -1;

  slider.on_value_changed([&](lvgl::Event& e) { value = slider.get_value(); });

  // Changing value programmatically usually doesn't fire event in LVGL
  // unless lvgl::AnimEnable::On or manually sent.
  // Let's send event manually to verify callback registration.
  slider.set_value(50, lvgl::AnimEnable::Off);
  (slider).send_event(lvgl::EventCode::ValueChanged);

  assert(value == 50);
  std::cout << "Slider Change Passed" << std::endl;
}

void test_switch_change() {
  std::cout << "Testing Switch Change..." << std::endl;
  lvgl::Switch sw;
  bool toggled = false;

  sw.on_value_changed([&](lvgl::Event& e) { toggled = true; });

  (sw).send_event(lvgl::EventCode::ValueChanged);
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

  std::cout << "Testing Checkbox Change..." << std::endl;
  lvgl::Checkbox cb;
  bool cb_checked = false;
  cb.on_value_changed([&](lvgl::Event& e) { cb_checked = true; });
  (cb).send_event(lvgl::EventCode::ValueChanged);
  assert(cb_checked);
  std::cout << "Checkbox Change Passed" << std::endl;

  std::cout << "Testing Arc Change..." << std::endl;
  lvgl::Arc arc;
  int arc_val = -1;
  arc.on_value_changed([&](lvgl::Event& e) { arc_val = 100; });
  (arc).send_event(lvgl::EventCode::ValueChanged);
  assert(arc_val == 100);
  std::cout << "Arc Change Passed" << std::endl;

  std::cout << "Testing Textarea Change..." << std::endl;
  lvgl::Textarea ta;
  bool ta_changed = false;
  ta.on_value_changed([&](lvgl::Event& e) { ta_changed = true; });
  (ta).send_event(lvgl::EventCode::ValueChanged);
  assert(ta_changed);
  std::cout << "Textarea Change Passed" << std::endl;

  std::cout << "Testing Spinbox Change..." << std::endl;
  lvgl::Spinbox sb;
  bool sb_changed = false;
  sb.on_value_changed([&](lvgl::Event& e) { sb_changed = true; });
  (sb).send_event(lvgl::EventCode::ValueChanged);
  assert(sb_changed);
  std::cout << "Spinbox Change Passed" << std::endl;

  std::cout << "Testing Calendar Change..." << std::endl;
  lvgl::Calendar cal;
  bool cal_changed = false;
  cal.on_value_changed([&](lvgl::Event& e) { cal_changed = true; });
  (cal).send_event(lvgl::EventCode::ValueChanged);
  assert(cal_changed);
  std::cout << "Calendar Change Passed" << std::endl;

  std::cout << "Testing Table Change..." << std::endl;
  lvgl::Table table;
  bool table_changed = false;
  table.on_value_changed([&](lvgl::Event& e) { table_changed = true; });
  (table).send_event(lvgl::EventCode::ValueChanged);
  assert(table_changed);
  std::cout << "Table Change Passed" << std::endl;

  return 0;
}
