#include "../misc/enums.h"

#if LVGL_VERSION_MAJOR == 9

#include <cassert>
#include <iostream>

#include "lv_conf_test.h"

void test_align() {
  lvgl::Align a = lvgl::Align::Center;
  assert(static_cast<lv_align_t>(a) == LV_ALIGN_CENTER);
  std::cout << "Align tests passed." << std::endl;
}

void test_dir() {
  lvgl::Dir d = lvgl::Dir::Hor;
  assert(static_cast<lv_dir_t>(d) == LV_DIR_HOR);
  std::cout << "Dir tests passed." << std::endl;
}

void test_part() {
  lvgl::Part p = lvgl::Part::Main;
  assert(static_cast<lv_part_t>(p) == LV_PART_MAIN);
  std::cout << "Part tests passed." << std::endl;
}

void test_state() {
  lvgl::State s1 = lvgl::State::Pressed;
  lvgl::State s2 = lvgl::State::Checked;
  lvgl::State s3 = s1 | s2;

  assert((static_cast<lv_state_t>(s3) & LV_STATE_PRESSED) != 0);
  assert((static_cast<lv_state_t>(s3) & LV_STATE_CHECKED) != 0);

  lvgl::State s4 = s3 & lvgl::State::Pressed;
  assert(s4 == lvgl::State::Pressed);

  std::cout << "State tests passed." << std::endl;
}

int main() {
  lv_init();
  test_align();
  test_dir();
  test_part();
  test_state();
  return 0;
}

#else

int main() { return 0; }

#endif
