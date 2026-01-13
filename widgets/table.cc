#include "table.h"

namespace lvgl {

Table::Table() : Table((Object*)nullptr) {}

Table::Table(Object *parent)
    : Object(lv_table_create(parent ? parent->raw() : nullptr)) {}

Table::Table(lv_obj_t *obj) : Object(obj) {}

void Table::set_cell_value(uint32_t row, uint32_t col, const char *txt) {
  if (obj_)
    lv_table_set_cell_value(obj_, row, col, txt);
}

void Table::set_row_count(uint32_t row_cnt) {
  if (obj_)
    lv_table_set_row_count(obj_, row_cnt);
}

void Table::set_column_count(uint32_t col_cnt) {
  if (obj_)
    lv_table_set_column_count(obj_, col_cnt);
}

void Table::set_column_width(uint32_t col_id, int32_t w) {
  if (obj_)
    lv_table_set_column_width(obj_, col_id, w);
}

void Table::set_cell_ctrl(uint32_t row, uint32_t col,
                          lv_table_cell_ctrl_t ctrl) {
  if (obj_)
    lv_table_set_cell_ctrl(obj_, row, col, ctrl);
}

void Table::clear_cell_ctrl(uint32_t row, uint32_t col,
                            lv_table_cell_ctrl_t ctrl) {
  if (obj_)
    lv_table_clear_cell_ctrl(obj_, row, col, ctrl);
}

void Table::set_cell_user_data(uint16_t row, uint16_t col, void *user_data) {
  if (obj_)
    lv_table_set_cell_user_data(obj_, row, col, user_data);
}

void Table::set_selected_cell(uint16_t row, uint16_t col) {
  if (obj_)
    lv_table_set_selected_cell(obj_, row, col);
}

const char *Table::get_cell_value(uint32_t row, uint32_t col) {
  return obj_ ? lv_table_get_cell_value(obj_, row, col) : nullptr;
}

uint32_t Table::get_row_count() {
  return obj_ ? lv_table_get_row_count(obj_) : 0;
}

uint32_t Table::get_column_count() {
  return obj_ ? lv_table_get_column_count(obj_) : 0;
}

int32_t Table::get_column_width(uint32_t col) {
  return obj_ ? lv_table_get_column_width(obj_, col) : 0;
}

bool Table::has_cell_ctrl(uint32_t row, uint32_t col,
                          lv_table_cell_ctrl_t ctrl) {
  return obj_ ? lv_table_has_cell_ctrl(obj_, row, col, ctrl) : false;
}

void Table::get_selected_cell(uint32_t *row, uint32_t *col) {
  if (obj_)
    lv_table_get_selected_cell(obj_, row, col);
}

void *Table::get_cell_user_data(uint16_t row, uint16_t col) {
  return obj_ ? lv_table_get_cell_user_data(obj_, row, col) : nullptr;
}

} // namespace lvgl
