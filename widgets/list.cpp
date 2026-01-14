#include "list.h"

#include "button.h"
#include "label.h"

#if LV_USE_LIST

namespace lvgl {

List::List() : List((Object*)nullptr) {}

List::List(Object* parent)
    : Object(lv_list_create(parent ? parent->raw() : nullptr)) {}

List::List(lv_obj_t* obj) : Object(obj) {}

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

}  // namespace lvgl

#endif // LV_USE_LIST
