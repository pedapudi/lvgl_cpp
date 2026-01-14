
#include <iostream>
#include <vector>

#include "../misc/animation.h"
#include "lvgl.h"

// Globals to be captured or modified
static int test_val = 0;
static bool is_completed = false;
static bool is_deleted = false;

void test_lambda_callbacks() {
  std::cout << "Testing Lambda Callbacks..." << std::endl;

  {
    lvgl::Animation a;
    a.set_values(0, 50);
    a.set_duration(50);
    a.set_var(&test_val);

    // Test capturing lambda
    int multiplier = 2;
    a.set_exec_cb([multiplier](void* var, int32_t v) {
      int* i_ptr = (int*)var;
      *i_ptr = v * multiplier;
    });

    a.set_completed_cb([]() {
      std::cout << "Callback: Completed" << std::endl;
      is_completed = true;
    });

    a.set_deleted_cb([]() {
      std::cout << "Callback: Deleted" << std::endl;
      is_deleted = true;
    });

    a.start();
  }  // wrapper destroys here, but animation should continue running because we
     // cloned the callback data!

  std::cout << "Animation started. Wrapper destroyed." << std::endl;

  // Simulate execution
  // 50ms duration. tick inc 10ms.
  for (int i = 0; i < 10; i++) {
    lv_tick_inc(10);
    lv_timer_handler();
    if (is_completed) break;
  }

  if (is_completed) {
    std::cout << "PASS: Animation completed." << std::endl;
  } else {
    std::cerr << "FAIL: Animation did not complete." << std::endl;
    exit(1);
  }

  // Verify value: should be around 50*2 = 100
  // It might settle at end value.
  if (test_val >= 90 && test_val <= 100) {
    std::cout << "PASS: Value updated correctly (Val=" << test_val << ")"
              << std::endl;
  } else {
    std::cerr << "FAIL: Value unexpected (Val=" << test_val << ")" << std::endl;
    exit(1);
  }

  // Check deletion.
  // lv_anim_handler calls deleted_cb when animation finishes and is removed.
  // If we reached completed, it should be deleted shortly or immediately.
  if (is_deleted) {
    std::cout << "PASS: Callback data deleted." << std::endl;
  } else {
    std::cout
        << "WARN: Deleted callback not yet called? Checking one more cycle..."
        << std::endl;
    lv_tick_inc(10);
    lv_timer_handler();
    if (is_deleted) {
      std::cout << "PASS: Callback data deleted." << std::endl;
    } else {
      std::cerr << "FAIL: Callback data NOT deleted! Memory leak suspected."
                << std::endl;
      exit(1);
    }
  }
}

int main() {
  lv_init();
  test_lambda_callbacks();
  return 0;
}
