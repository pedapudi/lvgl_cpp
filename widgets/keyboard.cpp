#include "keyboard.h"

#if LV_USE_KEYBOARD

namespace lvgl {

Keyboard::Keyboard()
    : Keyboard(static_cast<Object*>(nullptr), Ownership::Managed) {}

Keyboard::Keyboard(Object* parent, Ownership ownership)
    : Widget(lv_keyboard_create(parent ? parent->raw() : nullptr), ownership) {}

Keyboard::Keyboard(Object& parent) : Keyboard(&parent) {}

Keyboard::Keyboard(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

Keyboard& Keyboard::set_textarea(lv_obj_t* ta) {
  if (obj_) lv_keyboard_set_textarea(obj_, ta);
  return *this;
}

Keyboard& Keyboard::set_textarea(Object& ta) {
  if (obj_) lv_keyboard_set_textarea(obj_, ta.raw());
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
