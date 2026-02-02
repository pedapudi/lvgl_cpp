#include <cassert>
#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../lvgl_cpp.h"

// Helper to check flex flow equality
void test_flex_layout() {
  std::cout << "Testing Flex Layout..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Object container(&screen);

  // Set flex flow using proxy
  container.layout()
      .flex_flow(lvgl::FlexFlow::RowWrap)
      .flex_align(lvgl::FlexAlign::Center, lvgl::FlexAlign::Center,
                  lvgl::FlexAlign::SpaceBetween)
      .flex_grow(1);

  // Verify using C API getters (which are style properties)
  lv_flex_flow_t flow =
      lv_obj_get_style_flex_flow(container.raw(), LV_PART_MAIN);
  assert(flow == LV_FLEX_FLOW_ROW_WRAP);

  lv_flex_align_t main_place =
      lv_obj_get_style_flex_main_place(container.raw(), LV_PART_MAIN);
  assert(main_place == LV_FLEX_ALIGN_CENTER);

  lv_flex_align_t cross_place =
      lv_obj_get_style_flex_cross_place(container.raw(), LV_PART_MAIN);
  assert(cross_place == LV_FLEX_ALIGN_CENTER);

  lv_flex_align_t track_place =
      lv_obj_get_style_flex_track_place(container.raw(), LV_PART_MAIN);
  assert(track_place == LV_FLEX_ALIGN_SPACE_BETWEEN);

  uint8_t grow = lv_obj_get_style_flex_grow(container.raw(), LV_PART_MAIN);
  assert(grow == 1);

  std::cout << "PASS: Flex Layout applied correctly." << std::endl;
}

void test_grid_layout() {
  std::cout << "Testing Grid Layout..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Object container(&screen);

  static int32_t col_dsc[] = {100, 100, LV_GRID_TEMPLATE_LAST};
  static int32_t row_dsc[] = {50, 50, LV_GRID_TEMPLATE_LAST};

  container.layout()
      .grid_dsc(col_dsc, row_dsc)
      .grid_align(lvgl::GridAlign::Center, lvgl::GridAlign::End);

  // Verify Layout Style
  // lv_obj_set_grid_dsc_array sets style_grid_column_dsc_array etc. and layout
  // to GRID

  // Check that layout is GRID
  uint32_t layout = lv_obj_get_style_layout(container.raw(), LV_PART_MAIN);
  assert(layout == LV_LAYOUT_GRID);

  // Check descriptors
  // Note: Getters return the pointer
  /*
  const int32_t* cols = lv_obj_get_style_grid_column_dsc_array(container.raw(),
  LV_PART_MAIN); assert(cols == col_dsc);
  */

  std::cout << "PASS: Grid Layout applied correctly." << std::endl;
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_flex_layout();
  test_grid_layout();

  std::cout << "All LayoutProxy tests passed!" << std::endl;
  return 0;
}
