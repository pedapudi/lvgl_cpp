#include <cassert>
#include <iostream>

#include "../display/display.h"
#include "../misc/style.h"
#include "../widgets/button.h"
#include "../widgets/label.h"
#include "lvgl.h"

// Helper to check color equality
bool color_eq(lv_color_t c1, lv_color_t c2) {
  return c1.red == c2.red && c1.green == c2.green && c1.blue == c2.blue;
}

void test_fluent_chain() {
  std::cout << "Testing Fluent Chain..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Button btn(screen);

  // Chain multiple setters
  btn.style(lvgl::Part::Main)
      .bg_color(lvgl::Color::from_hex(0xFF0000))
      .radius(10)
      .border_width(2)
      .border_color(lvgl::Color::from_hex(0x0000FF));

  // Verify values using raw C API
  lv_obj_t* raw = btn.raw();

  // Check BG Color
  lv_color_t bg_color = lv_obj_get_style_bg_color(raw, LV_PART_MAIN);
  assert(color_eq(bg_color, lv_color_hex(0xFF0000)));

  // Check Radius
  int32_t radius = lv_obj_get_style_radius(raw, LV_PART_MAIN);
  assert(radius == 10);

  // Check Border Width
  int32_t border_width = lv_obj_get_style_border_width(raw, LV_PART_MAIN);
  assert(border_width == 2);

  // Check Border Color
  lv_color_t border_color = lv_obj_get_style_border_color(raw, LV_PART_MAIN);
  assert(color_eq(border_color, lv_color_hex(0x0000FF)));

  std::cout << "PASS: Fluent chain applied correctly." << std::endl;
}

void test_selector_usage() {
  std::cout << "Testing Selector Usage..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Button btn(screen);

  // Set styles for different parts/states
  // SANITY CHECK: Call C API directly first to verify behavior
  lv_obj_set_style_bg_color(btn.raw(), lv_color_hex(0x00FF00),
                            LV_PART_MAIN | LV_STATE_PRESSED);

  // Now try via Proxy (should overwrite or be identical)
  btn.style(lvgl::State::Pressed).bg_color(lvgl::Color::from_hex(0x00FF00));
  btn.style(lvgl::Part::Scrollbar).bg_color(lvgl::Color::from_hex(0x0000FF));

  // Verify Pressed state
  lv_color_t pressed_color = lv_obj_get_style_bg_color(btn.raw(), LV_PART_MAIN);
  std::cout << "Pressed Color: " << std::hex << (int)pressed_color.red << ","
            << (int)pressed_color.green << "," << (int)pressed_color.blue
            << std::dec << std::endl;
  assert(color_eq(pressed_color, lv_color_hex(0x00FF00)));

  // Verify Main state is NOT affected (should be default)
  // Note: Default might be theme dependent, but definitely not green
  lv_color_t main_color = lv_obj_get_style_bg_color(btn.raw(), LV_PART_MAIN);
  assert(!color_eq(main_color, lv_color_hex(0x00FF00)));

  // Verify Scrollbar part
  lv_color_t scroll_color =
      lv_obj_get_style_bg_color(btn.raw(), LV_PART_SCROLLBAR);
  assert(color_eq(scroll_color, lv_color_hex(0x0000FF)));

  std::cout << "PASS: Selectors applied correctly." << std::endl;
}

int main() {
  lv_init();
  // Use a dummy display buffer? Or create a real one?
  // tests/test_animation_timeline.cpp creates a display, so we should too.
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_fluent_chain();
  test_selector_usage();

  std::cout << "All tests passed!" << std::endl;
  return 0;
}
