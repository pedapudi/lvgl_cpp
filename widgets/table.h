#ifndef LVGL_CPP_WIDGETS_TABLE_H_
#define LVGL_CPP_WIDGETS_TABLE_H_


#include "../core/object.h"
#include "lvgl.h"

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

class Table : public Object {
public:
  /**
   * @brief Create a Table on the active screen.
   */
  Table();
  /**
   * @brief Create a Table with a parent.
   * @param parent The parent object.
   */
  explicit Table(Object *parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Table(lv_obj_t *obj);

  void set_cell_value(uint32_t row, uint32_t col, const char *txt);
  void set_row_count(uint32_t row_cnt);
  void set_column_count(uint32_t col_cnt);
  void set_column_width(uint32_t col_id, int32_t w);
  void set_cell_ctrl(uint32_t row, uint32_t col, lv_table_cell_ctrl_t ctrl);
  void clear_cell_ctrl(uint32_t row, uint32_t col, lv_table_cell_ctrl_t ctrl);
  void set_cell_user_data(uint16_t row, uint16_t col, void *user_data);
  void set_selected_cell(uint16_t row, uint16_t col);

  const char *get_cell_value(uint32_t row, uint32_t col);
  uint32_t get_row_count();
  uint32_t get_column_count();
  int32_t get_column_width(uint32_t col);
  bool has_cell_ctrl(uint32_t row, uint32_t col, lv_table_cell_ctrl_t ctrl);
  void get_selected_cell(uint32_t *row, uint32_t *col);
  void *get_cell_user_data(uint16_t row, uint16_t col);
};

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
} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_TABLE_H_
