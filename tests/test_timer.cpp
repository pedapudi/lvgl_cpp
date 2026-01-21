#include <iostream>
#include <string>
#include <vector>

#include "../misc/timer.h"
#include "lvgl.h"

// Variable to capture execution
static bool resumed = false;

void test_timer_resume() {
  std::cout << "Testing Timer Resume Handler..." << std::endl;
  resumed = false;

  // Set handler
  lvgl::Timer::set_resume_handler([]() {
    resumed = true;
    std::cout << "DEBUG: Resume handler called!" << std::endl;
  });

  // Disable timer handler
  std::cout << "Disabling timer..." << std::endl;
  // We create a timer instance just to access the method?
  // Actually Timer::enable is an instance method calling a global function.
  // But since it's just a wrapper, we can create a dummy timer or just use C
  // API if we wanted. Ideally Timer::enable should be static. But current API
  // is void enable(bool). Let's create a dummy timer just to call enable. It
  // mocks "using the timer system".

  {
    lvgl::Timer t;  // Doesn't create lv_timer unless args passed?
    // Default ctor: timer_(nullptr).
    t.enable(false);
  }

  if (resumed) {
    std::cerr << "FAIL: Resumed triggered early." << std::endl;
    exit(1);
  }

  // Enable timer handler - should trigger resume callback
  std::cout << "Enabling timer..." << std::endl;
  {
    lvgl::Timer t;
    t.enable(true);
  }

  if (resumed) {
    std::cout << "PASS: Resume handler triggered." << std::endl;
  } else {
    std::cerr << "FAIL: Resume handler NOT triggered." << std::endl;
    exit(1);
  }
}

void test_timer_clear_resume() {
  std::cout << "Testing Clear Resume Handler..." << std::endl;
  resumed = false;

  lvgl::Timer::clear_resume_handler();

  {
    lvgl::Timer t;
    t.enable(false);
    t.enable(true);
  }

  if (!resumed) {
    std::cout << "PASS: Resume handler cleared." << std::endl;
  } else {
    std::cerr << "FAIL: Resume handler triggered after clear." << std::endl;
    exit(1);
  }
}

int main() {
  lv_init();

  test_timer_resume();
  test_timer_clear_resume();

  std::cout << "\nAll Timer tests passed!" << std::endl;
  return 0;
}
