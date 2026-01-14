#include "button.h"

#if LV_USE_BUTTON

namespace lvgl {

Button::Button() : Button((Object*)nullptr) {}

Button::Button(Object* parent, Ownership ownership)
    : Object(lv_button_create(parent ? parent->raw() : nullptr), ownership) {}

Button::Button(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

}  // namespace lvgl

#endif  // LV_USE_BUTTON
