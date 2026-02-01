#include <cassert>
#include <iostream>

#include "../core/event.h"
#include "../core/object.h"
#include "../display/display.h"
#include "../widgets/button.h"
#include "lvgl.h"

using namespace lvgl;

static void test_event_basic() {
  printf("Testing basic event...\n");

  // Create a display to ensure lv_obj_create works
  std::cout << "Creating display..." << std::endl;
  Display display = Display::create(800, 600);

  std::cout << "Creating object..." << std::endl;
  Object obj;
  std::cout << "Object created." << std::endl;
  bool called = false;

  std::cout << "Adding callback..." << std::endl;
  obj.add_event_cb(EventCode::Clicked, [&](Event& e) {
    std::cout << "Callback invoked!" << std::endl;
    called = true;
    assert(e.get_code() == EventCode::Clicked);
    assert(e.get_target().raw() == obj.raw());
    // Current target should also be obj in this simple case
    assert(e.get_current_target().raw() == obj.raw());
  });

  std::cout << "Sending event..." << std::endl;
  lv_obj_send_event(obj.raw(), LV_EVENT_CLICKED, nullptr);
  std::cout << "Event sent." << std::endl;
  assert(called);
  std::cout << "Basic event passed." << std::endl;
}

static void test_event_param() {
  std::cout << "Testing event param..." << std::endl;
  Object obj;
  int value = 42;
  bool called = false;

  obj.add_event_cb(EventCode::ValueChanged, [&](Event& e) {
    called = true;
    int* p = e.get_param<int>();
    assert(p != nullptr);
    assert(*p == 42);
  });

  lv_obj_send_event(obj.raw(), LV_EVENT_VALUE_CHANGED, &value);
  assert(called);
  std::cout << "Event param passed." << std::endl;
}

int main() {
  lv_init();
  test_event_basic();
  test_event_param();
  std::cout << "All Event System tests passed." << std::endl;
  return 0;
}
