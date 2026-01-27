#include <cassert>
#include <cstdio>

#include "../core/object.h"
#include "lvgl.h"

using namespace lvgl;

// Simple test runner structure
static int tests_passed = 0;
static int tests_failed = 0;

#define TEST_CASE(name) void name()

#define ASSERT_TRUE(condition)                                        \
  if (!(condition)) {                                                 \
    printf("ASSERT_TRUE failed: %s at %s:%d\n", #condition, __FILE__, \
           __LINE__);                                                 \
    tests_failed++;                                                   \
    return;                                                           \
  }

#define ASSERT_EQ(a, b)                                                       \
  {                                                                           \
    auto val_a = (a);                                                         \
    auto val_b = (b);                                                         \
    if (val_a != val_b) {                                                     \
      printf("ASSERT_EQ failed: %d != %d at %s:%d\n", (int)val_a, (int)val_b, \
             __FILE__, __LINE__);                                             \
      tests_failed++;                                                         \
      return;                                                                 \
    }                                                                         \
  }

TEST_CASE(test_coordinate_transforms) {
  Object parent(lv_screen_active());
  parent.set_pos(10, 10).set_size(100, 100);

  Object child(&parent);
  child.set_pos(20, 20).set_size(50, 50);

  // Update layout to ensure coordinates are calculated
  parent.update_layout();
  child.update_layout();

  // Test get_coords() - absolute coordinates
  Area child_coords = child.get_coords();
  // Parent (10,10) + Child (20,20) = (30,30)
  ASSERT_EQ(child_coords.raw()->x1, 30);
  ASSERT_EQ(child_coords.raw()->y1, 30);
  ASSERT_EQ(child_coords.get_width(), 50);

  // Test transform_point
  // Note: lv_obj_transform_point only handles zoom/angle, not translation to
  // screen coords by default unless flags/matrix used differently. With no
  // zoom/angle, it should remain (5,5).
  Point p(5, 5);
  Point p_trans = child.transform_point(p, true, false);
  ASSERT_EQ(p_trans.x(), 5);
  ASSERT_EQ(p_trans.y(), 5);
}

TEST_CASE(test_scroll_getters) {
  Object obj(lv_screen_active());
  obj.set_size(100, 100);

  // Set scroll properties via proxy
  obj.scroll()
      .mode(ScrollbarMode::On)
      .dir(Dir::Ver)
      .snap(ScrollSnap::Center, ScrollSnap::End);

  // Verify via new getters
  ASSERT_EQ(static_cast<int>(obj.get_scrollbar_mode()),
            static_cast<int>(LV_SCROLLBAR_MODE_ON));
  ASSERT_EQ(static_cast<int>(obj.get_scroll_dir()),
            static_cast<int>(LV_DIR_VER));
  ASSERT_EQ(static_cast<int>(obj.get_scroll_snap_x()),
            static_cast<int>(LV_SCROLL_SNAP_CENTER));
  ASSERT_EQ(static_cast<int>(obj.get_scroll_snap_y()),
            static_cast<int>(LV_SCROLL_SNAP_END));
}

TEST_CASE(test_interaction_proxy) {
  Object screen(lv_screen_active());
  Object parent(&screen);
  parent.set_size(200, 200).set_pos(0, 0);
  Object btn(&parent);
  btn.set_size(100, 50);
  btn.set_pos(50, 50);
  btn.update_layout();  // Ensure coordinates are calculated

  // Use interaction proxy
  btn.interaction().set_ext_click_area(10);

  Area click_area = btn.get_click_area();
  // Normal area is 100x50. Extended by 10 on all sides.
  // Debug info
  int32_t w = btn.get_width();
  int32_t h = btn.get_height();
  int32_t cw = click_area.get_width();
  int32_t ch = click_area.get_height();
  printf("Btn Size: %d x %d\n", w, h);
  printf("Click Area: %d x %d (x1: %d, y1: %d, x2: %d, y2: %d)\n", cw, ch,
         click_area.raw()->x1, click_area.raw()->y1, click_area.raw()->x2,
         click_area.raw()->y2);

  ASSERT_EQ(cw, 120);
  ASSERT_EQ(ch, 70);

  // Hit test
  // Center of button: 50 + (100/2) = 100
  // Y: 50 + (50/2) = 75
  Point hit_p(100, 75);
  ASSERT_TRUE(btn.interaction().hit_test(hit_p));
}

TEST_CASE(test_tree_proxy) {
  Object screen = lvgl::Object(lv_screen_active());
  screen.clean();  // clear screen

  Object obj1(&screen);
  Object obj2(&screen);
  Object obj3(&screen);

  // Initial order: obj1 (index 0), obj2 (index 1), obj3 (index 2)
  ASSERT_EQ(obj1.get_index(), 0);
  ASSERT_EQ(obj2.get_index(), 1);
  ASSERT_EQ(obj3.get_index(), 2);

  // Swap obj1 and obj3 using proxy
  obj1.tree().swap(obj3);

  // New order: obj3 (0), obj2 (1), obj1 (2)
  ASSERT_EQ(obj3.get_index(), 0);
  ASSERT_EQ(obj2.get_index(), 1);
  ASSERT_EQ(obj1.get_index(), 2);

  // Move obj2 to index 0
  obj2.tree().move_to_index(0);
  // Order: obj2 (0), obj3 (1), obj1 (2)
  ASSERT_EQ(obj2.get_index(), 0);
  ASSERT_EQ(obj3.get_index(), 1);
}

int main() {
  lv_init();

  // Create a dummy display for testing
  static lv_display_t* disp = lv_display_create(800, 480);

  test_coordinate_transforms();
  test_scroll_getters();
  test_interaction_proxy();
  test_tree_proxy();

  printf("\nTests passed: %d\n", tests_passed - tests_failed);
  printf("Tests failed: %d\n", tests_failed);

  return tests_failed == 0 ? 0 : 1;
}
