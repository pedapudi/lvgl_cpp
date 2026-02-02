#include <cassert>
#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../lvgl_cpp.h"

void test_scroll_proxy() {
  std::cout << "Testing Scroll Proxy..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Object container(&screen);

  lv_obj_set_size(container.raw(), 100, 100);

  // Create large content to allow scrolling
  lvgl::Object content(&container);
  lv_obj_set_size(content.raw(), 200, 200);

  // Test ScrollProxy API
  container.scroll()
      .to(10, 20, lvgl::AnimEnable::Off)
      .mode(lvgl::ScrollbarMode::On)
      .snap(lvgl::ScrollSnap::Start, lvgl::ScrollSnap::End)
      .dir(lvgl::Dir::Ver);

  // Verify
  assert(lv_obj_get_scroll_x(container.raw()) == 10);
  assert(lv_obj_get_scroll_y(container.raw()) == 20);
  assert(lv_obj_get_scrollbar_mode(container.raw()) == LV_SCROLLBAR_MODE_ON);
  assert(lv_obj_get_scroll_snap_x(container.raw()) == LV_SCROLL_SNAP_START);
  assert(lv_obj_get_scroll_snap_y(container.raw()) == LV_SCROLL_SNAP_END);
  assert(lv_obj_get_scroll_dir(container.raw()) == LV_DIR_VER);

  std::cout << "PASS: Scroll Proxy basic functions." << std::endl;

  // Test getters (from Phase 9)
  assert(static_cast<lvgl::ScrollbarMode>(container.get_scrollbar_mode()) ==
         lvgl::ScrollbarMode::On);
  assert(static_cast<lvgl::Dir>(container.get_scroll_dir()) == lvgl::Dir::Ver);
  assert(static_cast<lvgl::ScrollSnap>(container.get_scroll_snap_x()) ==
         lvgl::ScrollSnap::Start);
  assert(static_cast<lvgl::ScrollSnap>(container.get_scroll_snap_y()) ==
         lvgl::ScrollSnap::End);
  std::cout << "PASS: Scroll getters." << std::endl;
}

int main() {
  lv_init();
  auto display = lvgl::Display::create(800, 480);

  test_scroll_proxy();

  std::cout << "All ScrollProxy tests passed!" << std::endl;
  return 0;
}
