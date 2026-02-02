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
  if (raw()) lv_keyboard_set_textarea(raw(), ta);
  return *this;
}

Keyboard& Keyboard::set_textarea(Object& ta) {
  if (raw()) lv_keyboard_set_textarea(raw(), ta.raw());
  return *this;
}

Keyboard& Keyboard::set_mode(Mode mode) {
  if (raw()) lv_keyboard_set_mode(raw(), static_cast<lv_keyboard_mode_t>(mode));
  return *this;
}

Keyboard& Keyboard::set_popovers(bool en) {
  if (raw()) lv_keyboard_set_popovers(raw(), en);
  return *this;
}

Keyboard& Keyboard::set_map(Mode mode, const char* const map[],
                            const lv_buttonmatrix_ctrl_t ctrl_map[]) {
  if (raw())
    lv_keyboard_set_map(raw(), static_cast<lv_keyboard_mode_t>(mode), map,
                        ctrl_map);
  return *this;
}

lv_obj_t* Keyboard::get_textarea() {
  return raw() ? lv_keyboard_get_textarea(raw()) : nullptr;
}

Keyboard::Mode Keyboard::get_mode() {
  return raw() ? static_cast<Mode>(lv_keyboard_get_mode(raw()))
               : Mode::TextLower;
}

bool Keyboard::get_popovers() {
  return raw() ? lv_keyboard_get_popovers(raw()) : false;
}

const char* const* Keyboard::get_map_array() {
  return raw() ? lv_keyboard_get_map_array(raw()) : nullptr;
}

uint32_t Keyboard::get_selected_button() {
  return raw() ? lv_keyboard_get_selected_button(raw())
               : LV_BUTTONMATRIX_BUTTON_NONE;
}

const char* Keyboard::get_button_text(uint32_t btn_id) {
  return raw() ? lv_keyboard_get_button_text(raw(), btn_id) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_KEYBOARD
