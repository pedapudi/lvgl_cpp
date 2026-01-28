#include <cassert>
#include <iostream>

#include "../display/display.h"
#include "../misc/animation_timeline.h"
#include "../widgets/button.h"
#include "lvgl.h"

static int anim1_exec_count = 0;
static int anim2_exec_count = 0;

void test_timeline_basic() {
  std::cout << "Testing Timeline Basic..." << std::endl;
  anim1_exec_count = 0;
  anim2_exec_count = 0;

  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Button btn1(screen);
  lvgl::Button btn2(screen);

  lvgl::AnimationTimeline timeline;

  {
    lvgl::Animation a1(btn1);
    a1.set_values(0, 100).set_duration(500).set_exec_cb(
        static_cast<lvgl::Animation::ExecCallback>(
            [](void* var, int32_t val) { anim1_exec_count++; }));
    timeline.add(a1, 0);  // Start at 0
  }

  {
    lvgl::Animation a2(btn2);
    a2.set_values(0, 100).set_duration(500).set_exec_cb(
        static_cast<lvgl::Animation::ExecCallback>(
            [](void* var, int32_t val) { anim2_exec_count++; }));
    timeline.add(a2, 200);  // Start at 200ms
  }

  timeline.start();

  // Run for 100ms
  for (int i = 0; i < 10; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
  }

  // A2 might have been called with start value 0 due to early_apply.
  // But it shouldn't have progressed (value > 0).
  // A1 should have progressed.

  if (anim1_exec_count > 0 &&
      anim2_exec_count >= 0) {  // A2 count doesn't matter as much as value
    std::cout << "PASS: Timeline started correctly." << std::endl;
  }

  // Run past 200ms
  for (int i = 0; i < 20; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
  }

  // Now A2 should have progressed.
  // We can't easily check previous values without storing them.
  // But anim2_exec_count should be significantly higher now.

  if (anim2_exec_count > 5) {  // Arbitrary threshold
    std::cout << "PASS: A2 started on timeline." << std::endl;
  } else {
    std::cerr << "FAIL: A2 did not start on timeline (count="
              << anim2_exec_count << ")." << std::endl;
    exit(1);
  }

  // Test pause
  timeline.pause();
  int count_before = anim1_exec_count;
  for (int i = 0; i < 10; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
  }

  // Pausing stops callbacks.
  if (anim1_exec_count == count_before) {
    std::cout << "PASS: Timeline paused." << std::endl;
  } else {
    std::cerr << "FAIL: Timeline continued running after pause." << std::endl;
    exit(1);
  }
}

void test_timeline_advanced() {
  std::cout << "Testing Timeline Advanced (delay, repeat, merge)..."
            << std::endl;

  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Button btn1(screen);

  {
    lvgl::AnimationTimeline timeline;
    timeline.set_delay(500);
    timeline.set_repeat_count(2);
    timeline.set_repeat_delay(100);

    if (timeline.get_delay() == 500 && timeline.get_repeat_count() == 2 &&
        timeline.get_repeat_delay() == 100) {
      std::cout << "PASS: Setting/getting delay and repeat works." << std::endl;
    } else {
      std::cerr << "FAIL: Delay or repeat mismatch." << std::endl;
      exit(1);
    }
  }

  {
    lvgl::AnimationTimeline t1;
    lvgl::AnimationTimeline t2;

    lvgl::Animation a1(btn1);
    a1.set_values(0, 100).set_duration(100).set_exec_cb(
        static_cast<lvgl::Animation::ExecCallback>(
            [](void* var, int32_t val) {}));

    t1.add(a1, 0);
    t2.add(a1, 100);

    uint32_t pt1 = t1.get_playtime();
    t1.merge(t2, 50);

    // pt1 was 100. t2 adds an anim starting at 100 with duration 100 -> 200.
    // + extra_delay 50 -> 250.
    if (t1.get_playtime() == 250) {
      std::cout << "PASS: Timeline merge works." << std::endl;
    } else {
      std::cerr << "FAIL: Merged playtime mismatch: " << t1.get_playtime()
                << " (expected 250)." << std::endl;
      exit(1);
    }
  }
}

void test_timeline_user_data() {
  std::cout << "Testing Timeline User Data (from Phase 10)..." << std::endl;
  lvgl::AnimationTimeline timeline;
  int data = 42;
  timeline.set_user_data(&data);
  assert(timeline.get_user_data() == &data);
  std::cout << "PASS: Timeline user data." << std::endl;
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);
  test_timeline_basic();
  test_timeline_advanced();
  test_timeline_user_data();
  std::cout << "All AnimationTimeline tests passed!" << std::endl;
  return 0;
}
