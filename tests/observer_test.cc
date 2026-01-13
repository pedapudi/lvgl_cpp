#include "../core/object.h" // For binding tests
#include "../core/observer.h"
#include <cassert>
#include <iostream>
#include <vector>

void test_basic_types() {
  std::cout << "Testing Basic Types..." << std::endl;

  // 1. IntSubject
  lvgl::IntSubject int_sub(10);
  assert(int_sub.get() == 10);
  int_sub.set(20);
  assert(int_sub.get() == 20);
  assert(int_sub.get_previous() == 10);

  std::cout << "Basic Types Passed." << std::endl;
}

int main() {
  std::cout << "Starting observer_test..." << std::endl;
  lv_init();
  std::cout << "lv_init done." << std::endl;

  test_basic_types();

  // Widget binding tests are disabled in this environment because
  // lv_display_create and lv_obj_create hang in the available headless
  // configuration.
  std::cout << "Skipping widget binding tests (requires display)." << std::endl;

  std::cout << "All Observer tests passed!" << std::endl;
  return 0;
}
