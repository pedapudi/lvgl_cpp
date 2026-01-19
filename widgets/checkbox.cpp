#include "checkbox.h"

namespace lvgl {

Checkbox::Checkbox() : Widget(lv_checkbox_create(lv_screen_active())) {}

Checkbox::Checkbox(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

Checkbox::Checkbox(Object* parent, Ownership ownership)
    : Widget(lv_checkbox_create(parent ? parent->raw() : lv_screen_active()),
             ownership) {}

Checkbox::Checkbox(Object& parent) : Widget(lv_checkbox_create(parent.raw())) {}

Checkbox::Checkbox(Object& parent, const char* text) : Checkbox(parent) {
  set_text(text);
}

Checkbox::Checkbox(Object& parent, const std::string& text) : Checkbox(parent) {
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
  return obj_ ? lv_checkbox_get_text(obj_) : "";
}

Checkbox& Checkbox::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  add_event_cb(cb, LV_EVENT_VALUE_CHANGED);
  return *this;
}

}  // namespace lvgl
