#include <cassert>
#include <iostream>
#include <vector>

#include "../core/group.h"
#include "../widgets/button.h"
#include "../widgets/label.h"
#include "lvgl.h"

using namespace lvgl;

void test_group_creation_destruction() {
  std::cout << "Testing Group Creation/Destruction..." << std::endl;
  {
    Group g(Group::Ownership::Managed);
    assert(g.raw() != nullptr);
  }  // g goes out of scope, should delete lv_group
  std::cout << "Group Creation/Destruction Passed" << std::endl;
}

void test_group_object_management() {
  std::cout << "Testing Group Object Management..." << std::endl;
  Group g;
  Button btn1;
  Button btn2;

  g.add_obj(btn1);
  g.add_obj(btn2);

  // Focus should be on one of them (likely the last added or first depending on
  // LVGL version) Actually LVGL doesn't automatically focus unless default
  // group is set or input device interaction happens. But we can check if they
  // are in the group.

  // Let's manually focus and check
  g.focus_obj(btn1);
  assert(g.get_focused() == btn1.raw());

  g.focus_next();
  assert(g.get_focused() == btn2.raw());

  g.focus_prev();
  assert(g.get_focused() == btn1.raw());

  g.remove_obj(btn1);
  // After removal, focus logic in LVGL might shift.

  g.remove_all_objs();
  // Should depend on LVGL implementation, but safe to call.

  std::cout << "Group Object Management Passed" << std::endl;
}

void test_default_group() {
  std::cout << "Testing Default Group..." << std::endl;
  Group g;
  Group::set_default(g);

  Group def = Group::get_default();
  assert(def.raw() == g.raw());

  // Clean up default group to avoid dangling pointer when g is destroyed
  lv_group_set_default(nullptr);
  std::cout << "Default Group Passed" << std::endl;
}

void test_focus_freeze_edit() {
  std::cout << "Testing Focus Freeze/Edit..." << std::endl;
  Group g;
  Button btn;
  g.add_obj(btn);
  g.focus_obj(btn);

  g.set_editing(true);
  // Hard to verify state without checking internal LVGL flags or visual output,
  // but ensures API calls are valid.

  g.focus_freeze(true);
  // Verify API call validity.

  g.focus_freeze(false);
  g.set_editing(false);

  std::cout << "Focus Freeze/Edit Passed" << std::endl;
}

int main() {
  lv_init();
  lv_display_create(800, 480);  // Ensure display exists for widgets

  test_group_creation_destruction();
  test_group_object_management();
  test_default_group();
  test_focus_freeze_edit();

  return 0;
}
