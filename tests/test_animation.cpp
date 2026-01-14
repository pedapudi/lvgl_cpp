
#include <iostream>
#include <vector>

#include "../misc/animation.h"
#include "../widgets/button.h"
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

static bool manual_stop_deleted = false;

void test_manual_stop() {
  std::cout << "Testing Manual Stop..." << std::endl;
  manual_stop_deleted = false;

  // Create animation and start it
  {
    lvgl::Animation a;
    a.set_values(0, 100);
    a.set_duration(1000);
    a.set_var(&test_val);

    a.set_deleted_cb([]() {
      std::cout << "Callback: Deleted (Manual Stop)" << std::endl;
      manual_stop_deleted = true;
    });

    a.start();
    std::cout << "Animation started." << std::endl;
  }

  // Now stop it manually using the variable
  // We need to match the variable pointer
  bool removed = lv_anim_del(&test_val, NULL);
  if (removed) {
    std::cout << "Animation removed successfully." << std::endl;
  } else {
    std::cerr << "FAIL: Animation not found to remove!" << std::endl;
    exit(1);
  }

  // deleted_cb should have been called
  if (manual_stop_deleted) {
    std::cout << "PASS: Callback data deleted after manual stop." << std::endl;
  } else {
    std::cerr << "FAIL: Callback data NOT deleted after manual stop!"
              << std::endl;
    exit(1);
  }
}

void test_object_oriented_api() {
  std::cout << "Testing Object Oriented API..." << std::endl;
  // Create a dummy object (Button)
  lvgl::Button obj;
  bool callback_called = false;

  {
    lvgl::Animation a(obj);
    a.set_values(0, 100);
    a.set_duration(50);
    // Verify the var is set correctly by checking if callback receives it
    a.set_exec_cb([&callback_called, &obj](void* var, int32_t v) {
      if (var == obj.raw()) {  // raw() returns lv_obj_t*
        callback_called = true;
      }
    });
    a.start();
  }

  // Tick to trigger animation step
  for (int i = 0; i < 10; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
    if (callback_called) break;
  }

  if (callback_called) {
    std::cout << "PASS: Animation constructor with Object works." << std::endl;
  } else {
    std::cerr << "FAIL: Animation constructor with Object failed." << std::endl;
    exit(1);
  }

  // Test set_var overload
  callback_called = false;
  {
    lvgl::Animation a;
    a.set_var(obj)
        .set_values(0, 100)
        .set_duration(50)
        .set_exec_cb([&callback_called, &obj](void* var, int32_t v) {
          if (var == obj.raw()) {
            callback_called = true;
          }
        })
        .start();
  }

  for (int i = 0; i < 10; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
    if (callback_called) break;
  }

  if (callback_called) {
    std::cout << "PASS: Animation::set_var(Object) works." << std::endl;
  } else {
    std::cerr << "FAIL: Animation::set_var(Object) failed." << std::endl;
    exit(1);
  }
}

void test_convenience_methods() {
  std::cout << "Testing Convenience Methods (set_exec_cb_y)..." << std::endl;
  lvgl::Button obj;
  // Position it at 0 initially
  obj.set_y(0);

  // Animate Y to 100
  lvgl::Animation(obj)
      .set_values(0, 100)
      .set_duration(50)
      .set_exec_cb(lvgl::Animation::Exec::Y())
      .set_path_cb(lvgl::Animation::Path::EaseIn())
      .start();

  // Simulate
  for (int i = 0; i < 10; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
  }

  // Check if Y changed
  if (obj.get_y() > 0) {
    std::cout << "PASS: set_exec_cb_y works (Y=" << obj.get_y() << ")"
              << std::endl;
  } else {
    std::cerr << "FAIL: set_exec_cb_y failed. Y did not change." << std::endl;
    exit(1);
  }

  // Checking Object::set_x/set_y directly
  obj.set_x(55);
  if (obj.get_x() == 55) {
    std::cout << "PASS: Object::set_x works." << std::endl;
  } else {
    std::cerr << "FAIL: Object::set_x failed." << std::endl;
    exit(1);
  }
}

void test_path_callback_lambda() {
  std::cout << "Testing Lambda Path Callback..." << std::endl;
  lvgl::Button obj;
  obj.set_y(0);

  bool path_called = false;

  lvgl::Animation::PathCallback cb =
      [&path_called](const lv_anim_t* a) -> int32_t {
    path_called = true;
    // Simple linear map for test
    return lv_map(a->act_time, 0, a->duration, a->start_value, a->end_value);
  };

  lvgl::Animation(obj)
      .set_values(0, 100)
      .set_duration(100)
      .set_exec_cb(lvgl::Animation::Exec::Y())
      .set_path_cb(cb)
      .start();

  // Simulate
  for (int i = 0; i < 5; ++i) {
    lv_tick_inc(25);
    lv_timer_handler();
  }

  if (path_called) {
    std::cout << "PASS: Lambda path callback called." << std::endl;
  } else {
    std::cerr << "FAIL: Lambda path callback NOT called." << std::endl;
    exit(1);
  }
}

void test_abstract_callbacks() {
  std::cout << "Testing Abstract Callbacks..." << std::endl;

  // Define concrete implementations
  class MyExec : public lvgl::AnimExecCallback {
   public:
    int count = 0;
    void operator()(void* var, int32_t val) override {
      count++;
      // Just modify opacity as a dummy action
      lv_obj_t* obj = (lv_obj_t*)var;
      lv_obj_set_style_opa(obj, (lv_opa_t)val, 0);
    }
  };

  class MyPath : public lvgl::AnimPathCallback {
   public:
    int count = 0;
    int32_t operator()(const lv_anim_t* a) override {
      count++;
      return lv_anim_path_linear(a);
    }
  };

  lvgl::Button obj;
  MyExec exec_cb;
  MyPath path_cb;

  lvgl::Animation(obj)
      .set_values(0, 255)
      .set_duration(50)
      .set_exec_cb([&exec_cb](void* var, int32_t val) { exec_cb(var, val); })
      .set_path_cb(
          [&path_cb](const lv_anim_t* a) -> int32_t { return path_cb(a); })
      .start();

  // Simulate
  for (int i = 0; i < 5; ++i) {
    lv_tick_inc(10);
    lv_timer_handler();
  }

  if (exec_cb.count > 0 && path_cb.count > 0) {
    std::cout << "PASS: Abstract callbacks called." << std::endl;
  } else {
    std::cerr << "FAIL: Abstract callbacks NOT called (Exec=" << exec_cb.count
              << ", Path=" << path_cb.count << ")." << std::endl;
    exit(1);
  }
}

int main() {
  lv_init();
  test_object_oriented_api();
  test_lambda_callbacks();
  test_manual_stop();
  test_convenience_methods();
  test_path_callback_lambda();
  test_abstract_callbacks();
  return 0;
}
