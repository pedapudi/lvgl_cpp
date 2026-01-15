#include "button.h"

#if LV_USE_BUTTON

namespace lvgl {

Button::Button() : Button((Object*)nullptr) {}

Button::Button(Object* parent, Ownership ownership)
    : Object(lv_button_create(parent ? parent->raw() : nullptr), ownership) {}

Button::Button(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Button& Button::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}

Button& Button::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}

Button& Button::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}

Button& Button::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_BUTTON
