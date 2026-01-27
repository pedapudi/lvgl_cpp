#include <cassert>
#include <iostream>

#include "../core/object.h"
#include "../misc/layout.h"

using namespace lvgl;

void test_grid_layout_builder() {
  std::cout << "Testing GridLayout Builder..." << std::endl;

  GridLayout grid;
  grid.add_column(100)
      .add_column(Size::Percent(50))
      .add_column(Grid::Template::Fr(1))
      .add_column(Grid::Template::Content);

  const int32_t* dsc = grid.col_dsc();

  assert(dsc[0] == 100);
  assert(dsc[1] == LV_PCT(50));
  assert(dsc[2] == LV_GRID_FR(1));
  assert(dsc[3] == LV_GRID_CONTENT);
  assert(dsc[4] == LV_GRID_TEMPLATE_LAST);

  grid.add_row(200).add_row(Grid::Template::Fr(2));

  const int32_t* row_dsc = grid.row_dsc();

  assert(row_dsc[0] == 200);
  assert(row_dsc[1] == LV_GRID_FR(2));
  assert(row_dsc[2] == LV_GRID_TEMPLATE_LAST);
}

void test_object_integration() {
  std::cout << "Testing Object Integration..." << std::endl;
  Object obj(lv_screen_active());
  GridLayout grid;
  grid.add_column(100).add_row(100);

  // Should not crash
  obj.set_grid_dsc_array(grid);
}

int main() {
  lv_init();
  test_grid_layout_builder();
  test_object_integration();
  std::cout << "[SUCCESS] GridLayout tests passed." << std::endl;
  return 0;
}
