#include "list.h"

#include "button.h"
#include "label.h"

#if LV_USE_LIST

namespace lvgl {

List::List() : List(static_cast<Object*>(nullptr), Ownership::Managed) {}

List::List(Object* parent, Ownership ownership)
    : Widget(lv_list_create(parent ? parent->raw() : nullptr), ownership) {}

List::List(Object& parent) : List(&parent) {}

List::List(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Label List::add_text(const char* txt) {
  return Label(raw() ? lv_list_add_text(raw(), txt) : nullptr);
}

Button List::add_button(const void* icon, const char* txt) {
  return Button(raw() ? lv_list_add_button(raw(), icon, txt) : nullptr);
}

const char* List::get_button_text(lv_obj_t* btn) {
  return raw() ? lv_list_get_button_text(raw(), btn) : "";
}

void List::set_button_text(lv_obj_t* btn, const char* txt) {
  if (raw()) lv_list_set_button_text(raw(), btn, txt);
}

}  // namespace lvgl

#endif  // LV_USE_LIST
