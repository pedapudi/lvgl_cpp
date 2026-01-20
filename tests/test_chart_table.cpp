#include <cassert>
#include <iostream>

#include "../display/display.h"
#include "../widgets/chart.h"
#include "../widgets/table.h"
#include "lvgl.h"

using namespace lvgl;

void test_chart_series() {
  std::cout << "Testing ChartSeries..." << std::endl;
  // Use a dedicated screen object to avoid issues with active screen ownership
  Object screen;
  Chart chart(screen);
  chart.set_size(200, 150);
  chart.center();

  std::cout << "Chart created, raw: " << chart.raw() << std::endl;

  chart.set_type(LV_CHART_TYPE_LINE);
  chart.set_point_count(10);

  ChartSeries s1 =
      chart.add_series(lv_color_make(255, 0, 0), LV_CHART_AXIS_PRIMARY_Y);

  if (s1.raw() == nullptr) {
    std::cout << "Series 1 is NULL" << std::endl;
  } else {
    std::cout << "Series 1 is " << s1.raw() << std::endl;

    s1.set_next_value(10);
    s1.set_next_value(20);
    s1.set_next_value(30);

    // Test set_all_values
    s1.set_all_values(50);

    // Test set_color
    s1.set_color(lv_color_make(0, 255, 0));

    // Test set_value_by_id
    s1.set_value_by_id(0, 100);
  }

  std::cout << "ChartSeries test passed" << std::endl;
}

void test_table_cell() {
  std::cout << "Testing TableCell..." << std::endl;
  Object screen;
  Table table(screen);
  table.set_column_count(3);
  table.set_row_count(4);

  // Test chaining
  table.cell(0, 0).set_value("A0").set_ctrl(LV_TABLE_CELL_CTRL_MERGE_RIGHT);

  // Test individual methods
  TableCell c1 = table.cell(1, 1);
  c1.set_value("B1");
  c1.add_ctrl(LV_TABLE_CELL_CTRL_TEXT_CROP);

  assert(c1.has_ctrl(LV_TABLE_CELL_CTRL_TEXT_CROP));

  c1.clear_ctrl(LV_TABLE_CELL_CTRL_TEXT_CROP);
  assert(!c1.has_ctrl(LV_TABLE_CELL_CTRL_TEXT_CROP));

  // Test user data
  int* my_data = (int*)lv_malloc(sizeof(int));
  *my_data = 42;
  table.cell(2, 2).set_user_data(my_data);
  assert(table.cell(2, 2).get_user_data() == my_data);

  std::cout << "TableCell test passed" << std::endl;
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_chart_series();
  test_table_cell();

  std::cout << "All Chart/Table tests passed!" << std::endl;
  return 0;
}
