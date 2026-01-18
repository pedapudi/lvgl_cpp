#include "button.h"

#if LV_USE_BUTTON

namespace lvgl {
Button::Button() : Button(static_cast<Object*>(nullptr), Ownership::Managed) {}

Button::Button(Object* parent, Ownership ownership)
    : Widget(lv_button_create(parent ? parent->raw() : nullptr), ownership) {}

Button::Button(Object& parent) : Button(&parent) {}

Button::Button(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}
}  // namespace lvgl

#endif  // LV_USE_BUTTON
