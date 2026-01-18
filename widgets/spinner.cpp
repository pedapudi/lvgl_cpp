#include "spinner.h"

#if LV_USE_SPINNER

namespace lvgl {

Spinner::Spinner()
    : Spinner(static_cast<Object*>(nullptr), Ownership::Managed) {}

Spinner::Spinner(Object* parent, Ownership ownership)
    : Widget(lv_spinner_create(parent ? parent->raw() : nullptr), ownership) {}

Spinner::Spinner(Object& parent) : Spinner(&parent) {}

Spinner::Spinner(Object& parent, uint32_t time, uint32_t arc_length)
    : Spinner(parent) {
  set_anim_params(time, arc_length);
}

Spinner::Spinner(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Spinner& Spinner::set_anim_params(uint32_t t, uint32_t angle) {
  if (obj_) lv_spinner_set_anim_params(obj_, t, angle);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_SPINNER
