#include "table.h"

#if LV_USE_TABLE

namespace lvgl {

// --- TableCell ---

TableCell& TableCell::set_value(const char* txt) {
  if (table_ && table_->raw()) {
    lv_table_set_cell_value(table_->raw(), row_, col_, txt);
  }
  return *this;
}

TableCell& TableCell::set_ctrl(lv_table_cell_ctrl_t ctrl) {
  if (table_ && table_->raw()) {
    lv_table_set_cell_ctrl(table_->raw(), row_, col_, ctrl);
  }
  return *this;
}

TableCell& TableCell::clear_ctrl(lv_table_cell_ctrl_t ctrl) {
  if (table_ && table_->raw()) {
    lv_table_clear_cell_ctrl(table_->raw(), row_, col_, ctrl);
  }
  return *this;
}

TableCell& TableCell::add_ctrl(lv_table_cell_ctrl_t ctrl) {
  if (table_ && table_->raw()) {
    lv_table_set_cell_ctrl(table_->raw(), row_, col_, ctrl);
  }
  return *this;
}

bool TableCell::has_ctrl(lv_table_cell_ctrl_t ctrl) const {
  return (table_ && table_->raw())
             ? lv_table_has_cell_ctrl(table_->raw(), row_, col_, ctrl)
             : false;
}

TableCell& TableCell::set_user_data(void* user_data) {
  if (table_ && table_->raw()) {
    lv_table_set_cell_user_data(table_->raw(), static_cast<uint16_t>(row_),
                                static_cast<uint16_t>(col_), user_data);
  }
  return *this;
}

void* TableCell::get_user_data() {
  return (table_ && table_->raw())
             ? lv_table_get_cell_user_data(table_->raw(), row_, col_)
             : nullptr;
}

// --- Table ---

Table::Table() : Table(static_cast<Object*>(nullptr), Ownership::Managed) {}

Table::Table(Object* parent, Ownership ownership)
    : Widget(lv_table_create(parent ? parent->raw() : nullptr), ownership) {}

Table::Table(Object& parent) : Table(&parent) {}

Table::Table(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

TableCell Table::cell(uint32_t row, uint32_t col) {
  return TableCell(this, row, col);
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

uint32_t Table::get_row_count() {
  return obj_ ? lv_table_get_row_count(obj_) : 0;
}

uint32_t Table::get_column_count() {
  return obj_ ? lv_table_get_column_count(obj_) : 0;
}

int32_t Table::get_column_width(uint32_t col) {
  return obj_ ? lv_table_get_column_width(obj_, col) : 0;
}

void Table::get_selected_cell(uint32_t* row, uint32_t* col) {
  if (obj_) lv_table_get_selected_cell(obj_, row, col);
}

Table& Table::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  add_event_cb(cb, LV_EVENT_VALUE_CHANGED);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_TABLE
