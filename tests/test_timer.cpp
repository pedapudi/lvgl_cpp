#include <iostream>
#include <vector>

#include "../misc/timer.h"
#include "lvgl.h"

static int callback_count = 0;
static int oneshot_count = 0;

void test_periodic_timer() {
  std::cout << "Testing Periodic Timer..." << std::endl;
  callback_count = 0;

  {
    auto t =
        lvgl::Timer::periodic(50, [](lvgl::Timer* t) { callback_count++; });

    for (int i = 0; i < 10; ++i) {
      lv_tick_inc(20);
      lv_timer_handler();
    }
  }
  // Timer destroyed here

  int count_after_destroy = callback_count;
  // Should verify it ran at least once
  if (callback_count > 0) {
    std::cout << "PASS: Periodic timer ran " << callback_count << " times."
              << std::endl;
  } else {
    std::cerr << "FAIL: Periodic timer did not run." << std::endl;
    exit(1);
  }

  // Verify it stopped
  for (int i = 0; i < 5; ++i) {
    lv_tick_inc(20);
    lv_timer_handler();
  }

  if (callback_count == count_after_destroy) {
    std::cout << "PASS: Periodic timer stopped after destruction." << std::endl;
  } else {
    std::cerr << "FAIL: Periodic timer continued running after destruction!"
              << std::endl;
    exit(1);
  }
}

void test_oneshot_timer() {
  std::cout << "Testing Oneshot Timer..." << std::endl;
  oneshot_count = 0;

  lvgl::Timer::oneshot(50, []() {
    oneshot_count++;
    std::cout << "Oneshot callback fired." << std::endl;
  });

  // Run efficiently
  for (int i = 0; i < 10; ++i) {
    lv_tick_inc(20);
    lv_timer_handler();
    if (oneshot_count > 0) break;
  }

  if (oneshot_count == 1) {
    std::cout << "PASS: Oneshot timer fired once." << std::endl;
  } else {
    std::cerr << "FAIL: Oneshot timer failed (count=" << oneshot_count << ")."
              << std::endl;
    exit(1);
  }

  // Verify it doesn't fire again
  for (int i = 0; i < 5; ++i) {
    lv_tick_inc(20);
    lv_timer_handler();
  }

  if (oneshot_count == 1) {
    std::cout << "PASS: Oneshot timer did not fire again." << std::endl;
  } else {
    std::cerr << "FAIL: Oneshot timer fired again!" << std::endl;
    exit(1);
  }
}

void test_move_semantics() {
  std::cout << "Testing Move Semantics..." << std::endl;
  callback_count = 0;

  lvgl::Timer t1 =
      lvgl::Timer::periodic(50, [](lvgl::Timer* t) { callback_count++; });

  // Move t1 to t2
  lvgl::Timer t2 = std::move(t1);

  // t1 should be empty/safe
  // t2 should run

  for (int i = 0; i < 5; ++i) {
    lv_tick_inc(20);
    lv_timer_handler();
  }

  if (callback_count > 0) {
    std::cout << "PASS: Moved timer executed." << std::endl;
  } else {
    std::cerr << "FAIL: Moved timer did not execute." << std::endl;
    exit(1);
  }

  // Assign t2 to t3
  lvgl::Timer t3;
  t3 = std::move(t2);

  int count_before = callback_count;
  for (int i = 0; i < 5; ++i) {
    lv_tick_inc(20);
    lv_timer_handler();
  }

  if (callback_count > count_before) {
    std::cout << "PASS: Move-assigned timer executed." << std::endl;
  } else {
    std::cerr << "FAIL: Move-assigned timer did not execute." << std::endl;
    exit(1);
  }
}

int main() {
  lv_init();
  test_periodic_timer();
  test_oneshot_timer();
  test_move_semantics();
  return 0;
}
