#include "checkbox.h"

#if LV_USE_CHECKBOX

namespace lvgl {

Checkbox::Checkbox()
    : Checkbox(static_cast<Object*>(nullptr), Ownership::Managed) {}

Checkbox::Checkbox(Object* parent, Ownership ownership)
    : Widget(lv_checkbox_create(parent ? parent->raw() : nullptr), ownership) {}

Checkbox::Checkbox(Object& parent) : Checkbox(&parent) {}

Checkbox::Checkbox(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

Checkbox::Checkbox(Object& parent, const char* text) : Checkbox(&parent) {
  set_text(text);
}

Checkbox::Checkbox(Object& parent, const std::string& text)
    : Checkbox(&parent) {
  set_text(text.c_str());
}

Checkbox& Checkbox::set_text(const char* txt) {
  if (obj_) lv_checkbox_set_text(obj_, txt);
  return *this;
}

Checkbox& Checkbox::set_text_static(const char* txt) {
  if (obj_) lv_checkbox_set_text_static(obj_, txt);
  return *this;
}

const char* Checkbox::get_text() const {
  return obj_ ? lv_checkbox_get_text(obj_) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_CHECKBOX
