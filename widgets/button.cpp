#include "button.h"

#if LV_USE_BUTTON

namespace lvgl {

Button::Button() : Button((Object*)nullptr) {}

Button::Button(Object* parent)
    : Object(lv_button_create(parent ? parent->raw() : nullptr)) {}

Button::Button(lv_obj_t* obj) : Object(obj) {}

}  // namespace lvgl

#endif // LV_USE_BUTTON
