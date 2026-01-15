#include "keyboard.h"

#if LV_USE_KEYBOARD

namespace lvgl {

Keyboard::Keyboard() : Keyboard((Object*)nullptr) {}

Keyboard::Keyboard(Object* parent, Ownership ownership)
    : ButtonMatrix(lv_keyboard_create(parent ? parent->raw() : nullptr)) {}

Keyboard::Keyboard(lv_obj_t* obj, Ownership ownership) : ButtonMatrix(obj) {}

Keyboard& Keyboard::set_textarea(lv_obj_t* ta) {
  if (obj_) lv_keyboard_set_textarea(obj_, ta);
  return *this;
}

Keyboard& Keyboard::set_textarea(Object* ta) {
  if (obj_) lv_keyboard_set_textarea(obj_, ta ? ta->raw() : nullptr);
  return *this;
}

Keyboard& Keyboard::set_mode(lv_keyboard_mode_t mode) {
  if (obj_) lv_keyboard_set_mode(obj_, mode);
  return *this;
}

Keyboard& Keyboard::set_popovers(bool en) {
  if (obj_) lv_keyboard_set_popovers(obj_, en);
  return *this;
}

Keyboard& Keyboard::set_map(lv_keyboard_mode_t mode, const char* const map[],
                            const lv_buttonmatrix_ctrl_t ctrl_map[]) {
  if (obj_) lv_keyboard_set_map(obj_, mode, map, ctrl_map);
  return *this;
}

Keyboard& Keyboard::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Keyboard& Keyboard::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Keyboard& Keyboard::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Keyboard& Keyboard::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Keyboard& Keyboard::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Keyboard& Keyboard::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Keyboard& Keyboard::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Keyboard& Keyboard::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

lv_obj_t* Keyboard::get_textarea() {
  return obj_ ? lv_keyboard_get_textarea(obj_) : nullptr;
}

lv_keyboard_mode_t Keyboard::get_mode() {
  return obj_ ? lv_keyboard_get_mode(obj_) : LV_KEYBOARD_MODE_TEXT_LOWER;
}

bool Keyboard::get_popovers() {
  return obj_ ? lv_keyboard_get_popovers(obj_) : false;
}

const char* const* Keyboard::get_map_array() {
  return obj_ ? lv_keyboard_get_map_array(obj_) : nullptr;
}

uint32_t Keyboard::get_selected_button() {
  return obj_ ? lv_keyboard_get_selected_button(obj_)
              : LV_BUTTONMATRIX_BUTTON_NONE;
}

const char* Keyboard::get_button_text(uint32_t btn_id) {
  return obj_ ? lv_keyboard_get_button_text(obj_, btn_id) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_KEYBOARD
