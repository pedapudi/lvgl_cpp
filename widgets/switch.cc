#include "switch.h"

namespace lvgl {

Switch::Switch() : Switch((Object*)nullptr) {}

Switch::Switch(Object* parent)
    : Object(lv_switch_create(parent ? parent->raw() : nullptr)) {}

Switch::Switch(lv_obj_t* obj) : Object(obj) {}

void Switch::set_orientation(lv_switch_orientation_t orientation) {
  if (obj_)
    lv_switch_set_orientation(obj_, orientation);
}

lv_switch_orientation_t Switch::get_orientation() const {
  return obj_ ? lv_switch_get_orientation(obj_) : LV_SWITCH_ORIENTATION_AUTO;
}

} // namespace lvgl
