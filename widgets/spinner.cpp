#include "spinner.h"

#if LV_USE_SPINNER

namespace lvgl {

Spinner::Spinner() : Spinner((Object*)nullptr) {}

Spinner::Spinner(Object* parent, Ownership ownership)
    : Object(lv_spinner_create(parent ? parent->raw() : nullptr), ownership) {}

Spinner::Spinner(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

void Spinner::set_anim_params(uint32_t t, uint32_t angle) {
  if (obj_) lv_spinner_set_anim_params(obj_, t, angle);
}

}  // namespace lvgl

#endif // LV_USE_SPINNER
