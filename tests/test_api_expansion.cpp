#include <cassert>
#include <iostream>

#include "../core/object.h"
#include "../indev/input_device.h"
#include "../misc/style.h"
#include "../widgets/button.h"
#include "../widgets/label.h"
#include "../lvgl_cpp.h"

void test_style_expansion() {
  std::cout << "Testing Style Expansion..." << std::endl;
  lvgl::Style style;

  // Animation
  style.anim_duration(1000);

  // Image
  style.image_opa(LV_OPA_50);
  style.image_recolor(lvgl::Color::from_hex(0xFF0000));

  // Line
  style.line_width(5);
  style.line_rounded(true);

  // Arc
  style.arc_width(10);
  style.arc_color(lvgl::Color::from_hex(0x00FF00));

  // Layout
  style.flex_flow(lvgl::FlexFlow::Row);
  style.flex_grow(1);
  style.grid_cell_column_span(2);

  // Verify via raw access or application
  lv_obj_t* obj = lv_obj_create(nullptr);
  lvgl::Object object(obj);
  object.add_style(style);

  assert(lv_obj_get_style_anim_duration(obj, LV_PART_MAIN) == 1000);
  assert(lv_obj_get_style_image_opa(obj, LV_PART_MAIN) == LV_OPA_50);
  assert(lv_obj_get_style_line_width(obj, LV_PART_MAIN) == 5);
  assert(lv_obj_get_style_arc_width(obj, LV_PART_MAIN) == 10);
  assert(lv_obj_get_style_flex_flow(obj, LV_PART_MAIN) == LV_FLEX_FLOW_ROW);

  std::cout << "Style Expansion Passed." << std::endl;
}

void test_object_layout() {
  std::cout << "Testing Object Layout..." << std::endl;
  lvgl::Object screen;
  lvgl::Object obj(&screen);

  // Test flex container properties
  obj.set_flex_flow(lvgl::FlexFlow::Column);
  obj.set_flex_align(lvgl::FlexAlign::Center, lvgl::FlexAlign::Start,
                     lvgl::FlexAlign::End);

  // Test flex item properties (requires parent)
  obj.set_flex_grow(2);

  assert(lv_obj_get_style_flex_flow(obj.raw(), LV_PART_MAIN) ==
         LV_FLEX_FLOW_COLUMN);
  assert(lv_obj_get_style_flex_grow(obj.raw(), LV_PART_MAIN) == 2);

  std::cout << "Object Layout Passed." << std::endl;
}

void test_label_manipulation() {
  std::cout << "Testing Label Manipulation..." << std::endl;
  lvgl::Object screen;
  lvgl::Label label(screen);
  label.set_text("Hello");

  label.ins_text(5, " World");
  assert(label.get_text() == "Hello World");

  label.cut_text(5, 6);
  assert(label.get_text() == "Hello");

  std::cout << "State::Pressed: " << static_cast<uint32_t>(lvgl::State::Pressed)
            << std::endl;
  std::cout << "Label Manipulation Passed." << std::endl;
}

void test_indev_config() {
  std::cout << "Testing Indev Config..." << std::endl;
  // Checking compilation mainly, runtime might fail if no indev driver
  // Use a mock indev if possible, or just create one manually?
  // lv_indev_create() needs a display? No.
  // But we need a driver.

  // We can create a dummy pointer input device
  lv_indev_t* raw_indev = lv_indev_create();
  lvgl::InputDevice indev(raw_indev, lvgl::Object::Ownership::Managed);
  indev.set_type(lvgl::IndevType::Pointer);

  indev.set_long_press_time(400);

  indev.set_scroll_limit(50);

  std::cout << "Indev Config Passed." << std::endl;
}

int main() {
  lv_init();
  lv_display_create(800, 600);  // Required for screen active

  test_style_expansion();
  test_object_layout();
  test_label_manipulation();
  test_indev_config();

  return 0;
}
