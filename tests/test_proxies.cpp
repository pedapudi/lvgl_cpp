#include <gtest/gtest.h>

#include "lvgl_cpp.h"

class ProxyTest : public ::testing::Test {
 protected:
  void SetUp() override {
    // LVGL is initialized in main or via a global fixture if needed.
  }
};

TEST_F(ProxyTest, TreeProxyOperations) {
  lvgl::Object parent;
  lvgl::Object child(&parent);

  EXPECT_EQ(parent.tree().get_child_count(), 1);
  EXPECT_EQ(child.tree().get_parent().raw(), parent.raw());
  EXPECT_EQ(parent.tree().get_child(0).raw(), child.raw());

  parent.tree().clean();
  EXPECT_EQ(parent.tree().get_child_count(), 0);
}

TEST_F(ProxyTest, InteractionProxyOperations) {
  lvgl::Object obj;

  obj.interaction().set_clickable(true);
  EXPECT_TRUE(obj.interaction().is_clickable());

  obj.interaction().set_clickable(false);
  EXPECT_FALSE(obj.interaction().is_clickable());

  // Hit test
  lvgl::Point p{10, 10};
  obj.set_size(100, 100);
  obj.set_pos(0, 0);
  // Note: hit test might need layout update
  obj.update_layout();
  EXPECT_TRUE(obj.interaction().hit_test(p));
}

TEST_F(ProxyTest, ZeroOverheadCheck) {
  lvgl::Object obj;
  // This is a compile-time/size check mostly.
  // TreeProxy and InteractionProxy should just be a pointer size.
  EXPECT_EQ(sizeof(lvgl::TreeProxy), sizeof(void*));
  EXPECT_EQ(sizeof(lvgl::InteractionProxy), sizeof(void*));
}
