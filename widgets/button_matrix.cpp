#include "button_matrix.h"

#if LV_USE_BUTTONMATRIX

namespace lvgl {

ButtonMatrix::ButtonMatrix()
    : ButtonMatrix(static_cast<Object*>(nullptr), Ownership::Managed) {}

ButtonMatrix::ButtonMatrix(Object* parent, Ownership ownership)
    : Widget(lv_buttonmatrix_create(parent ? parent->raw() : nullptr),
             ownership) {}

ButtonMatrix::ButtonMatrix(Object& parent) : ButtonMatrix(&parent) {}

ButtonMatrix::ButtonMatrix(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

ButtonMatrix::ButtonMatrix(Object& parent, const char* const map[])
    : ButtonMatrix(parent) {
  set_map(map);
}

ButtonMatrix& ButtonMatrix::set_map(const char* const map[]) {
  if (raw()) lv_buttonmatrix_set_map(raw(), map);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_ctrl_map(
    const lv_buttonmatrix_ctrl_t ctrl_map[]) {
  if (raw()) lv_buttonmatrix_set_ctrl_map(raw(), ctrl_map);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_selected_button(uint32_t btn_id) {
  if (raw()) lv_buttonmatrix_set_selected_button(raw(), btn_id);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_button_ctrl(uint32_t btn_id, Control ctrl) {
  if (raw())
    lv_buttonmatrix_set_button_ctrl(raw(), btn_id,
                                    static_cast<lv_buttonmatrix_ctrl_t>(ctrl));
  return *this;
}

ButtonMatrix& ButtonMatrix::clear_button_ctrl(uint32_t btn_id, Control ctrl) {
  if (raw())
    lv_buttonmatrix_clear_button_ctrl(
        raw(), btn_id, static_cast<lv_buttonmatrix_ctrl_t>(ctrl));
  return *this;
}

ButtonMatrix& ButtonMatrix::set_button_ctrl_all(Control ctrl) {
  if (raw())
    lv_buttonmatrix_set_button_ctrl_all(
        raw(), static_cast<lv_buttonmatrix_ctrl_t>(ctrl));
  return *this;
}

ButtonMatrix& ButtonMatrix::clear_button_ctrl_all(Control ctrl) {
  if (raw())
    lv_buttonmatrix_clear_button_ctrl_all(
        raw(), static_cast<lv_buttonmatrix_ctrl_t>(ctrl));
  return *this;
}

ButtonMatrix& ButtonMatrix::set_button_width(uint32_t btn_id, uint32_t width) {
  if (raw()) lv_buttonmatrix_set_button_width(raw(), btn_id, width);
  return *this;
}

ButtonMatrix& ButtonMatrix::set_one_checked(bool en) {
  if (raw()) lv_buttonmatrix_set_one_checked(raw(), en);
  return *this;
}

const char* const* ButtonMatrix::get_map() {
  return raw() ? lv_buttonmatrix_get_map(raw()) : nullptr;
}

uint32_t ButtonMatrix::get_selected_button() {
  return raw() ? lv_buttonmatrix_get_selected_button(raw())
               : LV_BUTTONMATRIX_BUTTON_NONE;
}

const char* ButtonMatrix::get_button_text(uint32_t btn_id) {
  return raw() ? lv_buttonmatrix_get_button_text(raw(), btn_id) : nullptr;
}

bool ButtonMatrix::has_button_ctrl(uint32_t btn_id, Control ctrl) {
  return raw() ? lv_buttonmatrix_has_button_ctrl(
                     raw(), btn_id, static_cast<lv_buttonmatrix_ctrl_t>(ctrl))
               : false;
}

bool ButtonMatrix::get_one_checked() {
  return raw() ? lv_buttonmatrix_get_one_checked(raw()) : false;
}

}  // namespace lvgl

#endif  // LV_USE_BUTTONMATRIX
