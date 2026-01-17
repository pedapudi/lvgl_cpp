#include "switch.h"

#if LV_USE_SWITCH

namespace lvgl {

Switch::Switch() : Object(lv_switch_create(nullptr), Ownership::Managed) {}

Switch::Switch(Object& parent, Ownership ownership)
    : Object(lv_switch_create(parent.raw()), ownership) {}

Switch::Switch(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Switch& Switch::set_orientation(lv_switch_orientation_t orientation) {
  if (obj_) lv_switch_set_orientation(obj_, orientation);
  return *this;
}

lv_switch_orientation_t Switch::get_orientation() const {
  return obj_ ? lv_switch_get_orientation(obj_) : LV_SWITCH_ORIENTATION_AUTO;
}

Switch& Switch::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Switch& Switch::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Switch& Switch::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Switch& Switch::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Switch& Switch::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Switch& Switch::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Switch& Switch::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Switch& Switch::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_SWITCH
