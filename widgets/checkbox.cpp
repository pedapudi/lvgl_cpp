#include "checkbox.h"

#if LV_USE_CHECKBOX

namespace lvgl {

Checkbox::Checkbox()
    : Widget(lv_checkbox_create(nullptr), Ownership::Managed) {}

Checkbox::Checkbox(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

Checkbox::Checkbox(Object* parent, Ownership ownership)
    : Widget(lv_checkbox_create(parent ? parent->raw() : nullptr), ownership) {}

Checkbox::Checkbox(Object& parent) : Checkbox(&parent) {}

Checkbox::Checkbox(Object& parent, const char* text) : Checkbox(&parent) {
  set_text(text);
}

Checkbox::Checkbox(Object& parent, const std::string& text)
    : Checkbox(&parent) {
  set_text(text.c_str());
}

Checkbox& Checkbox::set_text(const char* txt) {
  if (raw()) lv_checkbox_set_text(raw(), txt);
  return *this;
}

Checkbox& Checkbox::set_text_static(const char* txt) {
  if (raw()) lv_checkbox_set_text_static(raw(), txt);
  return *this;
}

const char* Checkbox::get_text() const {
  return raw() ? lv_checkbox_get_text(raw()) : nullptr;
}

}  // namespace lvgl

#endif  // LV_USE_CHECKBOX
