#include "spinner.h"

#if LV_USE_SPINNER

namespace lvgl {

Spinner::Spinner() : Object(lv_spinner_create(nullptr), Ownership::Managed) {}

Spinner::Spinner(Object& parent, Ownership ownership)
    : Object(lv_spinner_create(parent.raw()), ownership) {}

// Correction: implementing correct logic.
Spinner::Spinner(Object& parent, uint32_t time, uint32_t arc_length)
    : Spinner(parent) {
  set_anim_params(time, arc_length);
}

Spinner::Spinner(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Spinner& Spinner::set_anim_params(uint32_t t, uint32_t angle) {
  if (obj_) lv_spinner_set_anim_params(obj_, t, angle);
  return *this;
}

Spinner& Spinner::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Spinner& Spinner::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Spinner& Spinner::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Spinner& Spinner::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Spinner& Spinner::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Spinner& Spinner::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Spinner& Spinner::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Spinner& Spinner::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_SPINNER
