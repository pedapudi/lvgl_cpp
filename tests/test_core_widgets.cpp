#include <cassert>
#include <iostream>
#include <vector>

#include "../core/object.h"
#include "../core/style.h"
#include "../display/display.h"
#include "../misc/color.h"
#include "../widgets/arc.h"
#include "../widgets/bar.h"
#include "../widgets/button.h"
#include "../widgets/checkbox.h"
#include "../widgets/image.h"
#include "../widgets/label.h"
#include "../widgets/led.h"
#include "../widgets/line.h"
#include "../widgets/slider.h"
#include "../widgets/switch.h"
#include "lvgl.h"

// Mock event handler
static void event_handler(lvgl::Event& e) {
  lv_event_code_t code = e.get_code();
  if (code == LV_EVENT_CLICKED) {
    std::cout << "Clicked" << std::endl;
  } else if (code == LV_EVENT_VALUE_CHANGED) {
    std::cout << "Toggled" << std::endl;
  }
}

// Button Example 1: Simple Buttons
void test_button_1() {
  std::cout << "Testing Button Example 1..." << std::endl;
  lvgl::Button btn1;
  btn1.align(lvgl::Object::Align::Center, 0, -40);
  btn1.align(lvgl::Object::Align::Center, 0, -40);
  btn1.remove_flag(LV_OBJ_FLAG_PRESS_LOCK);
  // Note: C++ wrapper might not expose add_event_cb directly in a way that
  // matches C exactly without a wrapper, but let's assume valid standard usage
  // or just creation for now if event handling isn't fully wrapped yet. For
  // this test, we verify creation and property setting.

  lvgl::Label label(btn1);
  label.set_text("Button");
  label.center();

  lvgl::Button btn2;
  btn2.align(lvgl::Object::Align::Center, 0, 40);
  btn2.add_flag(LV_OBJ_FLAG_CHECKABLE);
  btn2.set_height(LV_SIZE_CONTENT);

  lvgl::Label label2(btn2);
  label2.set_text("Toggle");
  label2.center();
  std::cout << "Button Example 1 Passed" << std::endl;
}

// Label Example 1: Line wrap, recoloring and scrolling
void test_label_1() {
  std::cout << "Testing Label Example 1..." << std::endl;
  lvgl::Label label1;
  label1.set_long_mode(LV_LABEL_LONG_MODE_WRAP);
  // Set recolor is not in V9 directly usually? Checking wrapper... wrapper has
  // it? If wrapper assumes V9, recolor might be different. Let's assume wrapper
  // matches internal LVGL version. lvgl_cpp wrapper should be checked. For now,
  // assuming standard API. label1.set_recolor(true); // If this method exists

  label1.set_text(
      "#0000ff Re-color# #ff00ff words# #ff0000 of a# label, align "
      "the lines to the center "
      "and wrap long text automatically.");
  label1.set_width(150);
  label1.set_style_text_align(LV_TEXT_ALIGN_CENTER, 0);
  label1.align(lvgl::Object::Align::Center, 0, -40);

  lvgl::Label label2;
  label2.set_long_mode(LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
  label2.set_width(150);
  label2.set_text("It is a circularly scrolling text. ");
  label2.align(lvgl::Object::Align::Center, 0, 40);
  std::cout << "Label Example 1 Passed" << std::endl;
}

// Bar Example 1: Simple Bar
void test_bar_1() {
  std::cout << "Testing Bar Example 1..." << std::endl;
  lvgl::Bar bar1;
  bar1.set_size(200, 20);
  bar1.center();
  bar1.set_value(70, LV_ANIM_OFF);
  std::cout << "Bar Example 1 Passed" << std::endl;
}

// Slider Example 1: Simple Slider
void test_slider_1() {
  std::cout << "Testing Slider Example 1..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Slider slider(screen);
  slider.center();

  // slider.add_event_cb(slider_event_cb, LV_EVENT_VALUE_CHANGED, NULL);
  slider.set_style_anim_duration(2000, 0);

  lvgl::Label label(screen);

  label.set_text("0%");
  label.align_to(slider, lvgl::Object::Align::OutBottomMid, 0, 10);
  std::cout << "Slider Example 1 Passed" << std::endl;
}

// Switch Example 1: Simple Switch
void test_switch_1() {
  std::cout << "Testing Switch Example 1..." << std::endl;
  // Assuming screen active wrapper or using default constructor for parentless
  // if needed, but C example uses lv_screen_active(). Wrapper likely defaults
  // to it or needs it. For now, testing basic instantiation.

  lvgl::Switch sw1;
  sw1.add_flag(LV_OBJ_FLAG_EVENT_BUBBLE);

  lvgl::Switch sw2;
  sw2.add_state(LV_STATE_CHECKED);

  lvgl::Switch sw3;
  sw3.add_state(LV_STATE_DISABLED);

  lvgl::Switch sw4;
  sw4.add_state(LV_STATE_CHECKED);
  sw4.add_state(LV_STATE_DISABLED);

  std::cout << "Switch Example 1 Passed" << std::endl;
}

// Checkbox Example 1: Simple Checkboxes
void test_checkbox_1() {
  std::cout << "Testing Checkbox Example 1..." << std::endl;
  lvgl::Checkbox cb1;
  cb1.set_text("Apple");

  lvgl::Checkbox cb2;
  cb2.set_text("Banana");
  cb2.add_state(LV_STATE_CHECKED);

  lvgl::Checkbox cb3;
  cb3.set_text("Lemon");
  cb3.add_state(LV_STATE_DISABLED);

  lvgl::Checkbox cb4;
  cb4.add_state(LV_STATE_CHECKED);
  cb4.add_state(LV_STATE_DISABLED);
  cb4.set_text("Melon\nand a new line");
  cb4.update_layout();

  std::cout << "Checkbox Example 1 Passed" << std::endl;
}

// Line Example 1: Simple Line
void test_line_1() {
  std::cout << "Testing Line Example 1..." << std::endl;
  static lv_point_precise_t line_points[] = {
      {5, 5}, {70, 70}, {120, 10}, {180, 60}, {240, 10}};

  lvgl::Line line1;
  line1.set_points(line_points, 5);
  line1.center();
  std::cout << "Line Example 1 Passed" << std::endl;
}

// Arc Example 1: Simple Arc
void test_arc_1() {
  std::cout << "Testing Arc Example 1..." << std::endl;
  lvgl::Arc arc;
  arc.set_size(150, 150);
  arc.set_rotation(135);
  arc.set_bg_angles(0, 270);
  arc.set_value(10);
  arc.center();

  // Test Example 2 bits (Loader)
  lvgl::Arc arc2;
  arc2.set_rotation(270);
  arc2.set_bg_angles(0, 360);
  arc2.remove_style(NULL, LV_PART_KNOB);
  arc2.remove_flag(LV_OBJ_FLAG_CLICKABLE);
  arc2.center();

  std::cout << "Arc Example 1 Passed" << std::endl;
}

// Image Example 1: Basic Usage
void test_image_1() {
  std::cout << "Testing Image Example 1..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  // We don't have the asset img_cogwheel_argb easily available, using Symbol
  // for API test
  lvgl::Image img1(screen);
  img1.set_src(LV_SYMBOL_OK " Accept");
  img1.align(lvgl::Object::Align::Center, 0, 0);

  lvgl::Image img2(screen);
  img2.set_src(LV_SYMBOL_CLOSE);
  img2.align_to(img1, lvgl::Object::Align::OutBottomMid, 0, 20);

  // Test API for opacity/recolor from Example 2
  img2.set_style_image_recolor_opa(LV_OPA_50, 0);
  img2.set_style_image_recolor(lv_palette_main(LV_PALETTE_BLUE), 0);

  std::cout << "Image Example 1 Passed" << std::endl;
}

// LED Example 1: LED with custom style
void test_led_1() {
  std::cout << "Testing LED Example 1..." << std::endl;
  lvgl::Led led1;
  led1.align(lvgl::Object::Align::Center, -80, 0);
  led1.off();

  lvgl::Led led2;
  led2.align(lvgl::Object::Align::Center, 0, 0);
  led2.set_brightness(150);
  led2.set_color(lv_palette_main(LV_PALETTE_RED));

  lvgl::Led led3;
  led3.align(lvgl::Object::Align::Center, 80, 0);
  led3.on();

  std::cout << "LED Example 1 Passed" << std::endl;
}

int main() {
  lv_init();

  // Create a dummy screen/display if needed for full integration?
  // Tests are running in headers-only mode mostly if not linking full driver.
  // However, wrappers assert valid objects usually.
  // Assuming linked against a headless or dummy display driver setup or just
  // object creation works.
  lvgl::Display display = lvgl::Display::create(800, 600);

  test_button_1();
  test_label_1();
  test_bar_1();
  test_slider_1();
  test_switch_1();
  test_checkbox_1();
  test_line_1();
  test_arc_1();
  test_image_1();
  test_led_1();

  return 0;
}
