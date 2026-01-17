#include "checkbox.h"

#if LV_USE_CHECKBOX

namespace lvgl {

Checkbox::Checkbox()
    : Object(lv_checkbox_create(nullptr), Ownership::Managed) {}

Checkbox::Checkbox(Object& parent, Ownership ownership)
    : Object(lv_checkbox_create(parent.raw()), ownership) {}

Checkbox::Checkbox(Object& parent, const char* text) : Checkbox(parent) {
  set_text(text);
}

Checkbox::Checkbox(Object& parent, const std::string& text) : Checkbox(parent) {
  set_text(text.c_str());
}

Checkbox::Checkbox(lv_obj_t* obj, Ownership ownership)
    : Object(obj, ownership) {}

Checkbox& Checkbox::set_text(const char* txt) {
  if (raw()) lv_checkbox_set_text(raw(), txt);
  return *this;
}

Checkbox& Checkbox::set_text_static(const char* txt) {
  if (raw()) lv_checkbox_set_text_static(raw(), txt);
  return *this;
}

const char* Checkbox::get_text() const {
  return raw() ? lv_checkbox_get_text(raw()) : nullptr;
}

Checkbox& Checkbox::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Checkbox& Checkbox::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Checkbox& Checkbox::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Checkbox& Checkbox::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Checkbox& Checkbox::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Checkbox& Checkbox::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Checkbox& Checkbox::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Checkbox& Checkbox::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_CHECKBOX
