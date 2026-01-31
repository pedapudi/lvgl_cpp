#include <cassert>
#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../widgets/button.h"
#include "../widgets/checkbox.h"
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

void test_modern_scroll() {
  std::cout << "Testing modernized scroll API..." << std::endl;
  Object screen(lv_screen_active());
  Object container(&screen);
  container.set_size(100, 100);
  Object child(&container);
  child.set_size(200, 200);

  // Testing the new AnimEnable overload
  container.scroll_to(10, 20, AnimEnable::Off);
  assert(container.get_scroll_x() == 10);
  assert(container.get_scroll_y() == 20);

  // lv_obj_scroll_by(..., -5, ...) scrolls the content LEFT by 5,
  // which INCREASES the scroll offset (seeing more of the right).
  container.scroll_by(-5, -5, AnimEnable::Off);
  assert(container.get_scroll_x() == 15);
  assert(container.get_scroll_y() == 25);

  child.scroll_to_view(AnimEnable::Off);
}

void test_modern_display() {
  std::cout << "Testing modernized display API..." << std::endl;
  Display* disp = Display::get_default();

  disp->set_rotation(Display::Rotation::ROT_90);
  assert(disp->get_rotation() == Display::Rotation::ROT_90);

  disp->set_render_mode(Display::RenderMode::Direct);
  // No direct getter for render mode in LVGL v9, but we can call it.

  Object screen(lv_screen_active());
  disp->load_screen_anim(screen, Display::LoadAnim::FadeIn, 100, 0, false);
}

void test_modern_observer() {
  std::cout << "Testing modernized observer (Checked binding)..." << std::endl;
  IntSubject subject(0);
  Checkbox cb(lv_screen_active());

  // Use the new fluent binding helper
  cb.bind_checked(subject);
  assert(!cb.state().is_checked());

  subject.set(1);
  assert(cb.state().is_checked());

  subject.set(0);
  assert(!cb.state().is_checked());
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
  test_modern_scroll();
  test_modern_display();
  test_modern_observer();

  std::cout << "[SUCCESS] Object coverage tests passed." << std::endl;
  return 0;
}
