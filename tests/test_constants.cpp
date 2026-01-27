#include <cassert>
#include <iostream>

#include "lvgl.h"
#include "misc/constants.h"

int main() {
  lv_init();

  std::cout << "Testing Constants..." << std::endl;

  // Size Constants
  assert(lvgl::Size::Content == LV_SIZE_CONTENT);
  assert(lvgl::Size::Percent(50) == LV_PCT(50));
  assert(lvgl::Size::Percent(100) == LV_PCT(100));

  // Coord Constants
  assert(lvgl::Coord::Max == LV_COORD_MAX);

  // Radius Constants
  assert(lvgl::Radius::Circle == LV_RADIUS_CIRCLE);

  // Grid Constants
  assert(lvgl::Grid::Template::Content == LV_GRID_CONTENT);
  assert(lvgl::Grid::Template::Last == LV_GRID_TEMPLATE_LAST);
  assert(lvgl::Grid::Template::Fr(2) == LV_GRID_FR(2));

  std::cout << "[SUCCESS] Constants validated." << std::endl;
  return 0;
}
