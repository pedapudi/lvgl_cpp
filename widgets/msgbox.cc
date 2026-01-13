#include "msgbox.h"
#include "button.h"
#include "label.h"

namespace lvgl {

MsgBox::MsgBox() : MsgBox((Object *)nullptr) {}

MsgBox::MsgBox(Object* parent)
    : Object(lv_msgbox_create(parent ? parent->raw() : nullptr)) {}

MsgBox::MsgBox(lv_obj_t* obj) : Object(obj) {}

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
  if (obj_)
    lv_msgbox_close(obj_);
}

void MsgBox::close_async() {
  if (obj_)
    lv_msgbox_close_async(obj_);
}

} // namespace lvgl
