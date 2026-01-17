
#include "../core/observer.h"

#include <iostream>

#if LV_USE_OBSERVER
#include <cassert>

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
void test_cpp_callback() {
  std::cout << "Testing C++ Callback..." << std::endl;

  lvgl::IntSubject int_sub(0);
  int call_count = 0;
  int last_val = -1;

  // 1. Add observer with lambda capturing local variables
  lvgl::Observer* obs = int_sub.add_observer([&](lvgl::Observer* o) {
    call_count++;
    // We know the subject is an IntSubject.
    // In a real scenario, we might capture the subject reference or cast
    // o->get_target() if supported? But here we can just access int_sub because
    // we captured it by reference or it's in scope.
    last_val = int_sub.get();
  });

  assert(obs != nullptr);

  // Note: lv_subject_add_observer fires the callback immediately with current
  // value (0). So call_count should be 1, last_val should be 0.
  assert(call_count == 1);
  assert(last_val == 0);

  // 2. Trigger notification
  int_sub.set(42);
  assert(call_count == 2);
  assert(last_val == 42);

  int_sub.set(100);
  assert(call_count == 3);
  assert(last_val == 100);

  // 3. Remove observer
  delete obs;  // This should call remove() via destructor

  int_sub.set(200);
  assert(call_count == 3);  // Should not increase
  assert(last_val == 100);  // Should not change

  std::cout << "C++ Callback Passed." << std::endl;
}
#endif  // LV_USE_OBSERVER

int main() {
  std::cout << "Starting observer_test..." << std::endl;
  lv_init();
  std::cout << "lv_init done." << std::endl;

#if LV_USE_OBSERVER
  test_basic_types();
  test_cpp_callback();

  // Widget binding tests are disabled in this environment because
  // lv_display_create and lv_obj_create hang in the available headless
  // configuration.
  std::cout << "Skipping widget binding tests (requires display)." << std::endl;

  std::cout << "All Observer tests passed!" << std::endl;
#else
  std::cout << "LV_USE_OBSERVER is disabled. Skipping tests." << std::endl;
#endif  // LV_USE_OBSERVER
  return 0;
}
