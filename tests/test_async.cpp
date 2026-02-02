#include <iostream>

#include "../misc/async.h"
#include "../lvgl_cpp.h"

static int callback_count = 0;
static bool callback_executed = false;

void test_fire_and_forget() {
  std::cout << "Testing Fire-and-Forget Async..." << std::endl;
  callback_count = 0;

  auto result = lvgl::Async::call([]() {
    callback_count++;
    std::cout << "Fire-and-forget callback executed." << std::endl;
  });

  if (result != LV_RESULT_OK) {
    std::cerr << "FAIL: Async::call returned error." << std::endl;
    exit(1);
  }

  // Callback should not have executed yet
  if (callback_count != 0) {
    std::cerr << "FAIL: Callback executed before timer_handler." << std::endl;
    exit(1);
  }

  // Run timer handler to execute the async call
  for (int i = 0; i < 5; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
    if (callback_count > 0) break;
  }

  if (callback_count == 1) {
    std::cout << "PASS: Fire-and-forget async call executed." << std::endl;
  } else {
    std::cerr << "FAIL: Callback count = " << callback_count << std::endl;
    exit(1);
  }
}

void test_cancellable_execute() {
  std::cout << "Testing Cancellable Async (Execute)..." << std::endl;
  callback_executed = false;

  {
    auto handle = lvgl::Async::call_cancellable([]() {
      callback_executed = true;
      std::cout << "Cancellable callback executed." << std::endl;
    });

    if (!handle.valid()) {
      std::cerr << "FAIL: Handle is not valid." << std::endl;
      exit(1);
    }

    // Run timer handler to execute the async call
    for (int i = 0; i < 5; ++i) {
      lv_tick_inc(10);
      lv_timer_handler();
      if (callback_executed) break;
    }
  }  // Handle destroyed here

  if (callback_executed) {
    std::cout << "PASS: Cancellable async call executed." << std::endl;
  } else {
    std::cerr << "FAIL: Callback was not executed." << std::endl;
    exit(1);
  }
}

void test_cancel_before_execution() {
  std::cout << "Testing Cancel Before Execution..." << std::endl;
  callback_executed = false;

  {
    auto handle = lvgl::Async::call_cancellable([]() {
      callback_executed = true;
      std::cout << "THIS SHOULD NOT PRINT!" << std::endl;
    });

    // Cancel immediately
    bool cancelled = handle.cancel();
    if (!cancelled) {
      std::cerr << "FAIL: cancel() returned false." << std::endl;
      exit(1);
    }

    // Run timer handler - callback should NOT execute
    for (int i = 0; i < 5; ++i) {
      lv_tick_inc(10);
      lv_timer_handler();
    }
  }

  if (!callback_executed) {
    std::cout << "PASS: Cancelled async call did not execute." << std::endl;
  } else {
    std::cerr << "FAIL: Cancelled callback still executed!" << std::endl;
    exit(1);
  }
}

void test_handle_move() {
  std::cout << "Testing Handle Move Semantics..." << std::endl;
  callback_executed = false;

  lvgl::AsyncHandle handle2;
  {
    auto handle1 =
        lvgl::Async::call_cancellable([]() { callback_executed = true; });
    handle2 = std::move(handle1);

    // handle1 should be invalid after move
    // handle2 should be valid
  }

  if (!handle2.valid()) {
    std::cerr << "FAIL: Moved handle is not valid." << std::endl;
    exit(1);
  }

  // Cancel via the moved handle
  handle2.cancel();

  // Run timer handler
  for (int i = 0; i < 5; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
  }

  if (!callback_executed) {
    std::cout << "PASS: Move semantics work correctly." << std::endl;
  } else {
    std::cerr << "FAIL: Callback executed after cancel via moved handle."
              << std::endl;
    exit(1);
  }
}

void test_release_handle() {
  std::cout << "Testing Handle Release..." << std::endl;
  callback_executed = false;

  {
    auto handle =
        lvgl::Async::call_cancellable([]() { callback_executed = true; });

    // Release ownership - callback should still execute
    handle.release();

    if (handle.valid()) {
      std::cerr << "FAIL: Handle still valid after release." << std::endl;
      exit(1);
    }
  }  // Handle destroyed, but callback should not be cancelled

  // Run timer handler
  for (int i = 0; i < 5; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
    if (callback_executed) break;
  }

  if (callback_executed) {
    std::cout << "PASS: Released async call still executed." << std::endl;
  } else {
    std::cerr << "FAIL: Released callback did not execute." << std::endl;
    exit(1);
  }
}

int main() {
  lv_init();

  test_fire_and_forget();
  test_cancellable_execute();
  test_cancel_before_execution();
  test_handle_move();
  test_release_handle();

  std::cout << "\nAll Async tests passed!" << std::endl;
  return 0;
}
