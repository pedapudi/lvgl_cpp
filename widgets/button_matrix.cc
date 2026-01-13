#include "button_matrix.h"

namespace lvgl {

ButtonMatrix::ButtonMatrix() : ButtonMatrix((Object*)nullptr) {}

ButtonMatrix::ButtonMatrix(Object* parent)
    : Object(lv_buttonmatrix_create(parent ? parent->raw() : nullptr)) {}

ButtonMatrix::ButtonMatrix(lv_obj_t* obj) : Object(obj) {}

void ButtonMatrix::set_map(const char* const map[]) {
  if (obj_)
    lv_buttonmatrix_set_map(obj_, map);
}

void ButtonMatrix::set_ctrl_map(const lv_buttonmatrix_ctrl_t ctrl_map[]) {
  if (obj_)
    lv_buttonmatrix_set_ctrl_map(obj_, ctrl_map);
}

void ButtonMatrix::set_selected_button(uint32_t btn_id) {
  if (obj_)
    lv_buttonmatrix_set_selected_button(obj_, btn_id);
}

void ButtonMatrix::set_button_ctrl(uint32_t btn_id,
                                   lv_buttonmatrix_ctrl_t ctrl) {
  if (obj_)
    lv_buttonmatrix_set_button_ctrl(obj_, btn_id, ctrl);
}

void ButtonMatrix::clear_button_ctrl(uint32_t btn_id,
                                     lv_buttonmatrix_ctrl_t ctrl) {
  if (obj_)
    lv_buttonmatrix_clear_button_ctrl(obj_, btn_id, ctrl);
}

void ButtonMatrix::set_button_ctrl_all(lv_buttonmatrix_ctrl_t ctrl) {
  if (obj_)
    lv_buttonmatrix_set_button_ctrl_all(obj_, ctrl);
}

void ButtonMatrix::clear_button_ctrl_all(lv_buttonmatrix_ctrl_t ctrl) {
  if (obj_)
    lv_buttonmatrix_clear_button_ctrl_all(obj_, ctrl);
}

void ButtonMatrix::set_button_width(uint32_t btn_id, uint32_t width) {
  if (obj_)
    lv_buttonmatrix_set_button_width(obj_, btn_id, width);
}

void ButtonMatrix::set_one_checked(bool en) {
  if (obj_)
    lv_buttonmatrix_set_one_checked(obj_, en);
}

const char* const *ButtonMatrix::get_map() {
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

} // namespace lvgl
