#include "spinner.h"

#if LV_USE_SPINNER

namespace lvgl {

Spinner::Spinner() : Spinner((Object*)nullptr) {}

Spinner::Spinner(Object* parent)
    : Object(lv_spinner_create(parent ? parent->raw() : nullptr)) {}

Spinner::Spinner(lv_obj_t* obj) : Object(obj) {}

void Spinner::set_anim_params(uint32_t t, uint32_t angle) {
  if (obj_) lv_spinner_set_anim_params(obj_, t, angle);
}

}  // namespace lvgl

#endif // LV_USE_SPINNER
