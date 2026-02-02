#include <cassert>
#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../misc/style.h"
#include "../widgets/span.h"
#include "../lvgl_cpp.h"

void test_span_proxy() {
  std::cout << "Testing Span Proxy..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::SpanGroup spangroup(screen);

  spangroup.set_size(300, 300);
  spangroup.set_align(lvgl::TextAlign::Center);
  spangroup.set_mode(lvgl::SpanGroup::Mode::Break);

  // Add spans
  lvgl::Span s1 = spangroup.add_span();
  s1.set_text("Hello ");

  lvgl::Style style1;
  style1.text_color(lvgl::Palette::Red).text_decor(lvgl::TextDecor::Underline);
  s1.set_style(style1);

  lvgl::Span s2 = spangroup.add_span();
  s2.set_text("World!");

  lvgl::Style style2;
  style2.text_color(lvgl::Palette::Blue);
  s2.set_style(style2);

  // Verify
  assert(spangroup.get_span_count() == 2);
  assert(spangroup.get_align() == lvgl::TextAlign::Center);

  // Check accessing child by ID
  lvgl::Span child2 = spangroup.get_child(1);
  assert(child2.raw() == s2.raw());

  std::cout << "PASS: Span creation and manipulation." << std::endl;
}

int main() {
  lv_init();
  auto display = lvgl::Display::create(800, 480);

  test_span_proxy();

  std::cout << "All Span tests passed!" << std::endl;
  return 0;
}
