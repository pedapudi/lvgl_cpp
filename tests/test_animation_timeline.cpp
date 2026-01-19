#include <cassert>
#include <iostream>
#include <memory>
#include <vector>

#include "../core/object.h"
#include "../display/display.h"
#include "../misc/animation.h"
#include "../misc/animation_timeline.h"
#include "lvgl.h"  // Ensure includes are correct relative to include paths

using namespace lvgl;

void test_timeline_lifecycle() {
  std::cout << "Testing AnimationTimeline Lifecycle..." << std::endl;

  struct Tracker {
    bool& flag;
    Tracker(bool& f) : flag(f) {}
    ~Tracker() { flag = true; }
  };

  bool was_deleted = false;

  {
    AnimationTimeline timeline;

    // Scope for animation creation
    {
      auto tracker = std::make_shared<Tracker>(was_deleted);

      Animation anim;
      anim.set_var(nullptr);  // Just a dummy
      anim.set_values(0, 100);
      anim.set_duration(500);

      // Capture the shared pointer
      anim.set_exec_cb([tracker](void*, int32_t) {});

      timeline.add(0, anim);
      // tracker ref count should be at least 1 (inside timeline)
      // local 'tracker' is destroyed at end of this scope
    }

    // Check 1: Should NOT be deleted yet
    if (was_deleted) {
      std::cerr << "FAIL: Tracker deleted prematurely! Timeline failed to hold "
                   "ownership."
                << std::endl;
      exit(1);
    }
  }

  // Check 2: Timeline destroyed. Tracker should now be deleted.
  if (!was_deleted) {
    std::cerr << "FAIL: Tracker was NOT deleted after timeline destruction! "
                 "Memory Leak detected."
              << std::endl;
    exit(1);
  }

  std::cout << "AnimationTimeline Lifecycle Passed" << std::endl;
}

void test_animation_enhancements() {
  std::cout << "Testing Animation Enhancements..." << std::endl;
  Animation anim;
  anim.set_early_apply(true);

  // Just verify it doesn't crash and raw pointer works
  const lv_anim_t* raw = anim.raw();
  if (raw == nullptr) {
    std::cerr << "FAIL: raw() returned nullptr" << std::endl;
    exit(1);
  }

  // Test set_start_cb mechanism (not execution)
  bool started = false;
  anim.set_start_cb([&started]() { started = true; });

  std::cout << "Animation Enhancements Passed (Static Checks)" << std::endl;
}

int main() {
  lv_init();

  test_timeline_lifecycle();
  test_animation_enhancements();

  std::cout << "All Animation Timeline Tests Passed!" << std::endl;
  return 0;
}
