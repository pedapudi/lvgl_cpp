#include "checkbox.h"

namespace lvgl {

Checkbox::Checkbox() : Checkbox((Object*)nullptr) {}

Checkbox::Checkbox(Object* parent)
    : Object(lv_checkbox_create(parent ? parent->raw() : nullptr)) {}

Checkbox::Checkbox(lv_obj_t* obj) : Object(obj) {}

void Checkbox::set_text(const char* txt) {
  if (obj_) lv_checkbox_set_text(obj_, txt);
}

void Checkbox::set_text_static(const char* txt) {
  if (obj_) lv_checkbox_set_text_static(obj_, txt);
}

const char* Checkbox::get_text() const {
  return obj_ ? lv_checkbox_get_text(obj_) : nullptr;
}

}  // namespace lvgl
