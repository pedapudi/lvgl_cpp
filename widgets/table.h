#ifndef LVGL_CPP_WIDGETS_TABLE_H_
#define LVGL_CPP_WIDGETS_TABLE_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_TABLE
/**
 * @file table.h
 * @brief C++ Wrapper for LVGL Table Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Table widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Table : public Widget<Table> {
 public:
  Table();
  explicit Table(Object* parent, Ownership ownership = Ownership::Default);
  explicit Table(Object& parent);
  explicit Table(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Table& set_cell_value(uint32_t row, uint32_t col, const char* txt);
  Table& set_row_count(uint32_t row_cnt);
  Table& set_column_count(uint32_t col_cnt);
  Table& set_column_width(uint32_t col_id, int32_t w);
  Table& set_cell_ctrl(uint32_t row, uint32_t col, lv_table_cell_ctrl_t ctrl);
  Table& clear_cell_ctrl(uint32_t row, uint32_t col, lv_table_cell_ctrl_t ctrl);
  Table& set_cell_user_data(uint16_t row, uint16_t col, void* user_data);
  Table& set_selected_cell(uint16_t row, uint16_t col);

  /**
   * @brief Register a callback for the ValueChanged event.
   * @param cb The callback function.
   */
  Table& on_value_changed(std::function<void(lvgl::Event&)> cb);

  const char* get_cell_value(uint32_t row, uint32_t col);
  uint32_t get_row_count();
  uint32_t get_column_count();
  int32_t get_column_width(uint32_t col);
  bool has_cell_ctrl(uint32_t row, uint32_t col, lv_table_cell_ctrl_t ctrl);
  void get_selected_cell(uint32_t* row, uint32_t* col);
  void* get_cell_user_data(uint16_t row, uint16_t col);
};

}  // namespace lvgl

#endif  // LV_USE_TABLE
#endif  // LVGL_CPP_WIDGETS_TABLE_H_
