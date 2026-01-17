#include "win.h"

#include "button.h"
#include "label.h"

#if LV_USE_WIN

namespace lvgl {

Win::Win() : Object(lv_win_create(nullptr), Ownership::Managed) {}

Win::Win(Object& parent, Ownership ownership)
    : Object(lv_win_create(parent.raw()), ownership) {}

Win::Win(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Label Win::add_title(const char* txt) {
  return Label(obj_ ? lv_win_add_title(obj_, txt) : nullptr);
}

Button Win::add_button(const void* icon, int32_t btn_w) {
  return Button(obj_ ? lv_win_add_button(obj_, icon, btn_w) : nullptr);
}

Win& Win::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Win& Win::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Win& Win::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Win& Win::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Win& Win::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Win& Win::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Win& Win::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Win& Win::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

WinHeader Win::get_header() {
  return WinHeader(obj_ ? lv_win_get_header(obj_) : nullptr);
}

WinContent Win::get_content() {
  return WinContent(obj_ ? lv_win_get_content(obj_) : nullptr);
}

}  // namespace lvgl

#endif  // LV_USE_WIN
