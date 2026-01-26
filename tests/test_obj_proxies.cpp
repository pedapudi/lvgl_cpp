#include <cassert>
#include <iostream>
#include <vector>

#include "../core/object.h"
#include "../display/display.h"
#include "../widgets/button.h"
#include "../widgets/label.h"
#include "../widgets/span.h"
#include "lvgl.h"

using namespace lvgl;

static void test_event_proxy() {
  std::cout << "Testing EventProxy..." << std::endl;
  Object obj;
  bool called = false;

  obj.event().on_clicked([&](Event& e) { called = true; });

  lv_obj_send_event(obj.raw(), LV_EVENT_CLICKED, nullptr);
  assert(called);

  called = false;
  obj.event().send(LV_EVENT_CLICKED);
  assert(called);

  std::cout << "EventProxy passed." << std::endl;
}

static void test_state_proxy() {
  std::cout << "Testing StateProxy..." << std::endl;
  Object obj;

  obj.state().add(State::Checked);
  assert(obj.state().is(State::Checked));

  obj.state().remove(State::Checked);
  assert(!obj.state().is(State::Checked));

  obj.state().set(State::Pressed, true);
  assert(obj.state().is(State::Pressed));

  obj.state().set(State::Pressed, false);
  assert(!obj.state().is(State::Pressed));

  std::cout << "StateProxy passed." << std::endl;
}

static void test_group_proxy() {
  std::cout << "Testing GroupProxy..." << std::endl;
  Object obj;
  lv_group_t* g = lv_group_create();

  obj.group().add(g);
  assert(lv_obj_get_group(obj.raw()) == g);

  obj.group().focus();
  assert(obj.group().is_focused());

  obj.group().remove();
  assert(lv_obj_get_group(obj.raw()) == nullptr);

  lv_group_delete(g);
  std::cout << "GroupProxy passed." << std::endl;
}

static void test_span_proxy_shortcuts() {
  std::cout << "Testing Span/SpanGroup integration..." << std::endl;
  SpanGroup group;
  Span span = group.add_span();

  span.set_text("Hello");
  assert(std::string(span.get_text()) == "Hello");

  Style s;
  s.text_color(lv_color_hex(0xFF0000));
  span.style(s);

  std::cout << "Span integration passed." << std::endl;
}

int main() {
  lv_init();
  Display display = Display::create(800, 600);

  test_event_proxy();
  test_state_proxy();
  test_group_proxy();
  test_span_proxy_shortcuts();

  std::cout << "All Object Proxy tests passed." << std::endl;
  return 0;
}
