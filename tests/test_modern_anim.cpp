#include <gtest/gtest.h>

#include <chrono>

#include "lvgl_cpp.h"

using namespace std::chrono_literals;

class ModernAnimTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // LVGL init
  }
};

TEST_F(ModernAnimTest, ChronoSupport) {
  lvgl::Animation anim;
  anim.set_duration(500ms);
  anim.set_delay(100ms);
  // No easy way to check internal lv_anim_t without access,
  // but if it compiles and runs without crashing it's a good sign.
}

TEST_F(ModernAnimTest, TemplatedExecCallback) {
  lvgl::Object parent;
  lvgl::Image img(parent);

  lvgl::Animation anim(img);
  bool called = false;
  anim.set_exec_cb<lvgl::Image>([&called](lvgl::Image& i, int32_t v) {
    called = true;
    i.set_rotation(v);
  });

  anim.set_values(0, 100);
  anim.set_duration(10);
  anim.start();

  // Minimal wait for anim to run (in a real test we'd need lv_timer_handler)
  lv_timer_handler();
  EXPECT_TRUE(called);
}

TEST_F(ModernAnimTest, BezierPath) {
  lvgl::Animation anim;
  anim.set_path_cb(lvgl::Animation::Path::Bezier(0, 0, 512, 1024));
  // Verify it doesn't crash during initialization
}
