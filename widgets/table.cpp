#include "table.h"

#if LV_USE_TABLE

namespace lvgl {

Table::Table() : Object(lv_table_create(nullptr), Ownership::Managed) {}

Table::Table(Object& parent, Ownership ownership)
    : Object(lv_table_create(parent.raw()), ownership) {}

Table::Table(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Table& Table::set_cell_value(uint32_t row, uint32_t col, const char* txt) {
  if (obj_) lv_table_set_cell_value(obj_, row, col, txt);
  return *this;
}

Table& Table::set_row_count(uint32_t row_cnt) {
  if (obj_) lv_table_set_row_count(obj_, row_cnt);
  return *this;
}

Table& Table::set_column_count(uint32_t col_cnt) {
  if (obj_) lv_table_set_column_count(obj_, col_cnt);
  return *this;
}

Table& Table::set_column_width(uint32_t col_id, int32_t w) {
  if (obj_) lv_table_set_column_width(obj_, col_id, w);
  return *this;
}

Table& Table::set_cell_ctrl(uint32_t row, uint32_t col,
                            lv_table_cell_ctrl_t ctrl) {
  if (obj_) lv_table_set_cell_ctrl(obj_, row, col, ctrl);
  return *this;
}

Table& Table::clear_cell_ctrl(uint32_t row, uint32_t col,
                              lv_table_cell_ctrl_t ctrl) {
  if (obj_) lv_table_clear_cell_ctrl(obj_, row, col, ctrl);
  return *this;
}

Table& Table::set_cell_user_data(uint16_t row, uint16_t col, void* user_data) {
  if (obj_) lv_table_set_cell_user_data(obj_, row, col, user_data);
  return *this;
}

Table& Table::set_selected_cell(uint16_t row, uint16_t col) {
  if (obj_) lv_table_set_selected_cell(obj_, row, col);
  return *this;
}

Table& Table::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Table& Table::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Table& Table::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Table& Table::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Table& Table::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Table& Table::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Table& Table::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Table& Table::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

const char* Table::get_cell_value(uint32_t row, uint32_t col) {
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

void Table::get_selected_cell(uint32_t* row, uint32_t* col) {
  if (obj_) lv_table_get_selected_cell(obj_, row, col);
}

void* Table::get_cell_user_data(uint16_t row, uint16_t col) {
  return obj_ ? lv_table_get_cell_user_data(obj_, row, col) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_TABLE
