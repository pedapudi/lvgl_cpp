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

class Table;  // Forward declaration

/**
 * @brief Proxy class for a table cell.
 */
class TableCell {
 public:
  TableCell(Table* table, uint32_t row, uint32_t col)
      : table_(table), row_(row), col_(col) {}

  TableCell& set_value(const char* txt);
  TableCell& set_ctrl(lv_table_cell_ctrl_t ctrl);
  TableCell& clear_ctrl(lv_table_cell_ctrl_t ctrl);
  TableCell& add_ctrl(lv_table_cell_ctrl_t ctrl);
  bool has_ctrl(lv_table_cell_ctrl_t ctrl) const;
  TableCell& set_user_data(void* user_data);
  void* get_user_data();

 private:
  Table* table_;
  uint32_t row_;
  uint32_t col_;
};

class Table : public Widget<Table> {
 public:
  Table();
  explicit Table(Object* parent, Ownership ownership = Ownership::Default);
  explicit Table(Object& parent);
  explicit Table(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Get a cell proxy object.
   * @param row The row index.
   * @param col The column index.
   * @return TableCell proxy.
   */
  TableCell cell(uint32_t row, uint32_t col);

  Table& set_row_count(uint32_t row_cnt);
  Table& set_column_count(uint32_t col_cnt);
  Table& set_column_width(uint32_t col_id, int32_t w);

  // Removed legacy individual cell methods in favor of TableCell

  /**
   * @brief Register a callback for the ValueChanged event.
   * @param cb The callback function.
   */
  Table& on_value_changed(std::function<void(lvgl::Event&)> cb);

  uint32_t get_row_count();
  uint32_t get_column_count();
  int32_t get_column_width(uint32_t col);
  void get_selected_cell(uint32_t* row, uint32_t* col);

  /**
   * @brief Get the value of a cell.
   * @param row Row index.
   * @param col Column index.
   * @return The cell value as a string.
   */
  const char* get_cell_value(uint32_t row, uint32_t col) const;

  /**
   * @brief Set the cell value using a formatted string.
   * @param row Row index.
   * @param col Column index.
   * @param fmt Format string.
   */
  Table& set_cell_value_fmt(uint32_t row, uint32_t col, const char* fmt, ...);

  /**
   * @brief Set the selected cell.
   * @param row Row index.
   * @param col Column index.
   */
  Table& set_selected_cell(uint32_t row, uint32_t col);
};

}  // namespace lvgl

#endif  // LV_USE_TABLE
#endif  // LVGL_CPP_WIDGETS_TABLE_H_
