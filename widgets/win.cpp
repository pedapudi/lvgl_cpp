#include "win.h"

#include "button.h"
#include "label.h"

#if LV_USE_WIN

namespace lvgl {

Win::Win() : Widget(lv_win_create(lv_screen_active()), Ownership::Managed) {}

Win::Win(Object& parent, Ownership ownership)
    : Widget(lv_win_create(parent.raw()), ownership) {}

Win::Win(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Label Win::add_title(const char* txt) {
  return Label(obj_ ? lv_win_add_title(obj_, txt) : nullptr);
}

Button Win::add_button(const void* icon, int32_t btn_w) {
  return Button(obj_ ? lv_win_add_button(obj_, icon, btn_w) : nullptr);
}

WinHeader Win::get_header() {
  return WinHeader(obj_ ? lv_win_get_header(obj_) : nullptr);
}

WinContent Win::get_content() {
  return WinContent(obj_ ? lv_win_get_content(obj_) : nullptr);
}

}  // namespace lvgl

#endif  // LV_USE_WIN
