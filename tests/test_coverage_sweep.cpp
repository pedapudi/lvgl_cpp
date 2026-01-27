#include <cassert>
#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../widgets/button.h"
#include "../widgets/label.h"

using namespace lvgl;

void test_has_class() {
  std::cout << "Testing has_class()..." << std::endl;
  Object screen(lv_screen_active());
  Button btn(screen);
  Label label(btn);

  assert(btn.has_class<Button>());
  assert(!btn.has_class<Label>());

  assert(label.has_class<Label>());
  assert(!label.has_class<Button>());

  assert(btn.has_class<Object>());
}

void test_is_visible() {
  std::cout << "Testing is_visible()..." << std::endl;
  Object screen(lv_screen_active());
  Button btn(screen);

  // By default, widgets are visible
  // Note: visibility depends on parent visibility and specific flags.
  // Let's force it visible/hidden to be sure.
  btn.remove_flag(ObjFlag::Hidden);
  assert(btn.is_visible());

  btn.add_flag(ObjFlag::Hidden);
  assert(!btn.is_visible());
}

void test_get_state() {
  std::cout << "Testing get_state()..." << std::endl;
  Object screen(lv_screen_active());
  Button btn(screen);

  // Initial state check
  assert((static_cast<lv_state_t>(btn.get_state()) & LV_STATE_CHECKED) == 0);

  btn.add_state(State::Checked);
  assert((static_cast<lv_state_t>(btn.get_state()) & LV_STATE_CHECKED) ==
         LV_STATE_CHECKED);

  btn.remove_state(State::Checked);
  assert((static_cast<lv_state_t>(btn.get_state()) & LV_STATE_CHECKED) == 0);
}

void test_get_screen() {
  std::cout << "Testing get_screen()..." << std::endl;
  Object screen(lv_screen_active());
  Button btn(screen);
  Label label(btn);

  Object btn_screen = btn.get_screen();
  assert(btn_screen.raw() == lv_screen_active());

  Object label_screen = label.get_screen();
  assert(label_screen.raw() == lv_screen_active());
}

void test_get_display() {
  std::cout << "Testing get_display()..." << std::endl;
  Object screen(lv_screen_active());
  Button btn(screen);

  lv_display_t* disp = btn.get_display();
  assert(disp != nullptr);
  assert(disp == lv_display_get_default());
}

int main() {
  lv_init();

  // Create a dummy display for the test
  Display display = Display::create(800, 480);

  // Creating/deleting objects requires memory management to be initialized
  // Depending on lv_conf.h, lv_init might be sufficient or we might need memory
  // allocator setup. Assuming lv_init() is enough for default config.

  test_has_class();
  test_is_visible();
  test_get_state();
  test_get_screen();
  test_get_display();

  std::cout << "[SUCCESS] Object coverage tests passed." << std::endl;
  return 0;
}
