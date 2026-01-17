#include "list.h"

#include "button.h"
#include "label.h"

#if LV_USE_LIST

namespace lvgl {

List::List() : Object(lv_list_create(nullptr), Ownership::Managed) {}

List::List(Object& parent, Ownership ownership)
    : Object(lv_list_create(parent.raw()), ownership) {}

List::List(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Label List::add_text(const char* txt) {
  return Label(obj_ ? lv_list_add_text(obj_, txt) : nullptr);
}

Button List::add_button(const void* icon, const char* txt) {
  return Button(obj_ ? lv_list_add_button(obj_, icon, txt) : nullptr);
}

const char* List::get_button_text(lv_obj_t* btn) {
  return obj_ ? lv_list_get_button_text(obj_, btn) : "";
}

void List::set_button_text(lv_obj_t* btn, const char* txt) {
  if (obj_) lv_list_set_button_text(obj_, btn, txt);
}

List& List::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
List& List::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
List& List::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
List& List::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
List& List::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
List& List::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
List& List::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
List& List::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_LIST
