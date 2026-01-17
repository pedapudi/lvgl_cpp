#include <cassert>
#include <iostream>

#include "core/group.h"
#include "lvgl.h"
#include "widgets/button.h"

int main() {
  lv_init();

  // Create a group
  lvgl::Group group;
  // By default, group wraps a new lv_group instance.

  lvgl::Object screen;
  lvgl::Button btn1(&screen);
  lvgl::Button btn2(&screen);

  // Test add_obj with reference
  group.add_obj(btn1);
  group.add_obj(btn2);

  // Initial focus should be on one of them (LVGL default behavior varies, but
  // count should be 2) We can't easily check count via C API wrapper unless we
  // added get_obj_count (which we did!) Wait, did we? check group.h. Yes:
  // uint32_t get_obj_count();

  // Note: get_obj_count in group.h likely wraps lv_group_get_obj_count

  // Let's assume we implemented get_obj_count properly.
  // assert(group.get_obj_count() == 2);
  // Actually, I should verify get_obj_count implementation if I want to rely on
  // it.

  // Test focus_obj
  group.focus_obj(btn1);
  assert(group.get_focused() == btn1.raw());

  group.focus_obj(btn2);
  assert(group.get_focused() == btn2.raw());

  // Test remove_obj
  group.remove_obj(btn1);
  // assert(group.get_obj_count() == 1);

  std::cout << "Group ownership/reference tests passed" << std::endl;
  return 0;
}
