#include "msgbox.h"

#include "button.h"
#include "label.h"

#if LV_USE_MSGBOX

namespace lvgl {

MsgBox::MsgBox() : MsgBox(static_cast<Object*>(nullptr), Ownership::Managed) {}

MsgBox::MsgBox(Object* parent, Ownership ownership)
    : Widget(lv_msgbox_create(parent ? parent->raw() : nullptr), ownership) {}

MsgBox::MsgBox(Object& parent) : MsgBox(&parent) {}

MsgBox::MsgBox(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Label MsgBox::add_title(const char* title) {
  return Label(obj_ ? lv_msgbox_add_title(obj_, title) : nullptr,
               Ownership::Unmanaged);
}

Button MsgBox::add_header_button(const void* icon) {
  return Button(obj_ ? lv_msgbox_add_header_button(obj_, icon) : nullptr,
                Ownership::Unmanaged);
}

Label MsgBox::add_text(const char* text) {
  return Label(obj_ ? lv_msgbox_add_text(obj_, text) : nullptr,
               Ownership::Unmanaged);
}

Button MsgBox::add_footer_button(const char* text) {
  return Button(obj_ ? lv_msgbox_add_footer_button(obj_, text) : nullptr,
                Ownership::Unmanaged);
}

Button MsgBox::add_close_button() {
  return Button(obj_ ? lv_msgbox_add_close_button(obj_) : nullptr,
                Ownership::Unmanaged);
}

MsgBoxHeader MsgBox::get_header() {
  return MsgBoxHeader(obj_ ? lv_msgbox_get_header(obj_) : nullptr,
                      Ownership::Unmanaged);
}

MsgBoxFooter MsgBox::get_footer() {
  return MsgBoxFooter(obj_ ? lv_msgbox_get_footer(obj_) : nullptr,
                      Ownership::Unmanaged);
}

MsgBoxContent MsgBox::get_content() {
  return MsgBoxContent(obj_ ? lv_msgbox_get_content(obj_) : nullptr,
                       Ownership::Unmanaged);
}

Label MsgBox::get_title() {
  return Label(obj_ ? lv_msgbox_get_title(obj_) : nullptr,
               Ownership::Unmanaged);
}

void MsgBox::close() {
  if (obj_) {
    lv_msgbox_close(obj_);
    // msgbox_close deletes the object instantly.
    // We must invalidate our wrapper to avoid double-free or use-after-free.
    obj_ = nullptr;
  }
}

void MsgBox::close_async() {
  if (obj_) {
    lv_msgbox_close_async(obj_);
    // Async close will delete it later. The delete hook mechanism in Object
    // should handle the notification when LVGL eventually deletes it.
    // However, for safety, one might consider invalidating immediately if no
    // further usage is expected? No, standard practice is to rely on the hook
    // for async deletion.
  }
}

}  // namespace lvgl

#endif  // LV_USE_MSGBOX
