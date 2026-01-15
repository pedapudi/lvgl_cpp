#include "msgbox.h"

#include "button.h"
#include "label.h"

#if LV_USE_MSGBOX

namespace lvgl {

MsgBox::MsgBox() : MsgBox((Object*)nullptr) {}

MsgBox::MsgBox(Object* parent, Ownership ownership)
    : Object(lv_msgbox_create(parent ? parent->raw() : nullptr), ownership) {}

MsgBox::MsgBox(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Label MsgBox::add_title(const char* title) {
  return Label(obj_ ? lv_msgbox_add_title(obj_, title) : nullptr);
}

Button MsgBox::add_header_button(const void* icon) {
  return Button(obj_ ? lv_msgbox_add_header_button(obj_, icon) : nullptr);
}

Label MsgBox::add_text(const char* text) {
  return Label(obj_ ? lv_msgbox_add_text(obj_, text) : nullptr);
}

Button MsgBox::add_footer_button(const char* text) {
  return Button(obj_ ? lv_msgbox_add_footer_button(obj_, text) : nullptr);
}

Button MsgBox::add_close_button() {
  return Button(obj_ ? lv_msgbox_add_close_button(obj_) : nullptr);
}

MsgBox& MsgBox::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
MsgBox& MsgBox::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
MsgBox& MsgBox::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
MsgBox& MsgBox::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
MsgBox& MsgBox::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
MsgBox& MsgBox::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
MsgBox& MsgBox::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
MsgBox& MsgBox::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

MsgBoxHeader MsgBox::get_header() {
  return MsgBoxHeader(obj_ ? lv_msgbox_get_header(obj_) : nullptr);
}

MsgBoxFooter MsgBox::get_footer() {
  return MsgBoxFooter(obj_ ? lv_msgbox_get_footer(obj_) : nullptr);
}

MsgBoxContent MsgBox::get_content() {
  return MsgBoxContent(obj_ ? lv_msgbox_get_content(obj_) : nullptr);
}

Label MsgBox::get_title() {
  return Label(obj_ ? lv_msgbox_get_title(obj_) : nullptr);
}

void MsgBox::close() {
  if (obj_) lv_msgbox_close(obj_);
}

void MsgBox::close_async() {
  if (obj_) lv_msgbox_close_async(obj_);
}

}  // namespace lvgl

#endif  // LV_USE_MSGBOX
