#include <cassert>
#include <iostream>
#include <vector>

#include "../core/object.h"
#include "../display/display.h"
#include "../misc/style.h"
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

static void test_interaction_proxy_extra() {
  std::cout << "Testing InteractionProxy extra..." << std::endl;
  Object parent;
  Object btn(&parent);
  btn.set_size(100, 50);
  btn.update_layout();

  btn.interaction().set_ext_click_area(10);
  Area click_area = btn.get_click_area();
  assert(click_area.get_width() == 120);
  assert(click_area.get_height() == 70);

  Point hit_p(50, 25);
  assert(btn.interaction().hit_test(hit_p));
  std::cout << "InteractionProxy extra passed." << std::endl;
}

static void test_tree_proxy_extra() {
  std::cout << "Testing TreeProxy extra..." << std::endl;
  Object parent;
  Object obj1(&parent);
  Object obj2(&parent);
  Object obj3(&parent);

  assert(obj1.get_index() == 0);
  assert(obj2.get_index() == 1);
  assert(obj3.get_index() == 2);

  obj1.tree().swap(obj3);
  assert(obj3.get_index() == 0);
  assert(obj1.get_index() == 2);

  obj2.tree().move_to_index(0);
  assert(obj2.get_index() == 0);
  std::cout << "TreeProxy extra passed." << std::endl;
}

int main() {
  lv_init();
  Display display = Display::create(800, 600);

  test_event_proxy();
  test_state_proxy();
  test_group_proxy();
  test_span_proxy_shortcuts();
  test_interaction_proxy_extra();
  test_tree_proxy_extra();

  std::cout << "All Object Proxy tests passed." << std::endl;
  return 0;
}
