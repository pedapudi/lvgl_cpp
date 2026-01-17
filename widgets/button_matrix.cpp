#include "button_matrix.h"

#if LV_USE_BUTTONMATRIX

namespace lvgl {

ButtonMatrix::ButtonMatrix() : ButtonMatrix((Object*)nullptr) {}

ButtonMatrix::ButtonMatrix(Object* parent, Ownership ownership)
    : Object(lv_buttonmatrix_create(parent ? parent->raw() : nullptr),
             ownership) {}

ButtonMatrix::ButtonMatrix(lv_obj_t* obj, Ownership ownership)
    : Object(obj, ownership) {}

ButtonMatrix::ButtonMatrix(Object* parent, const char* const map[])
    : ButtonMatrix(parent) {
  set_map(map);
}

ButtonMatrix& ButtonMatrix::set_map(const char* const map[]) {
  if (obj_) lv_buttonmatrix_set_map(obj_, map);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_ctrl_map(
    const lv_buttonmatrix_ctrl_t ctrl_map[]) {
  if (obj_) lv_buttonmatrix_set_ctrl_map(obj_, ctrl_map);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_selected_button(uint32_t btn_id) {
  if (obj_) lv_buttonmatrix_set_selected_button(obj_, btn_id);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_button_ctrl(uint32_t btn_id,
                                            lv_buttonmatrix_ctrl_t ctrl) {
  if (obj_) lv_buttonmatrix_set_button_ctrl(obj_, btn_id, ctrl);
  return *this;
}

ButtonMatrix& ButtonMatrix::clear_button_ctrl(uint32_t btn_id,
                                              lv_buttonmatrix_ctrl_t ctrl) {
  if (obj_) lv_buttonmatrix_clear_button_ctrl(obj_, btn_id, ctrl);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_button_ctrl_all(lv_buttonmatrix_ctrl_t ctrl) {
  if (obj_) lv_buttonmatrix_set_button_ctrl_all(obj_, ctrl);
  return *this;
}

ButtonMatrix& ButtonMatrix::clear_button_ctrl_all(lv_buttonmatrix_ctrl_t ctrl) {
  if (obj_) lv_buttonmatrix_clear_button_ctrl_all(obj_, ctrl);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_button_width(uint32_t btn_id, uint32_t width) {
  if (obj_) lv_buttonmatrix_set_button_width(obj_, btn_id, width);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_one_checked(bool en) {
  if (obj_) lv_buttonmatrix_set_one_checked(obj_, en);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
ButtonMatrix& ButtonMatrix::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
ButtonMatrix& ButtonMatrix::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
ButtonMatrix& ButtonMatrix::align(Align align, int32_t x_ofs,
                                  int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
ButtonMatrix& ButtonMatrix::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
ButtonMatrix& ButtonMatrix::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
ButtonMatrix& ButtonMatrix::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
ButtonMatrix& ButtonMatrix::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

const char* const* ButtonMatrix::get_map() {
  return obj_ ? lv_buttonmatrix_get_map(obj_) : nullptr;
}

uint32_t ButtonMatrix::get_selected_button() {
  return obj_ ? lv_buttonmatrix_get_selected_button(obj_)
              : LV_BUTTONMATRIX_BUTTON_NONE;
}

const char* ButtonMatrix::get_button_text(uint32_t btn_id) {
  return obj_ ? lv_buttonmatrix_get_button_text(obj_, btn_id) : nullptr;
}

bool ButtonMatrix::has_button_ctrl(uint32_t btn_id,
                                   lv_buttonmatrix_ctrl_t ctrl) {
  return obj_ ? lv_buttonmatrix_has_button_ctrl(obj_, btn_id, ctrl) : false;
}

bool ButtonMatrix::get_one_checked() {
  return obj_ ? lv_buttonmatrix_get_one_checked(obj_) : false;
}

}  // namespace lvgl

#endif  // LV_USE_BUTTONMATRIX
