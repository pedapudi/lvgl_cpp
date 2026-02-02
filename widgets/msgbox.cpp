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
  return Label(raw() ? lv_msgbox_add_title(raw(), title) : nullptr,
               Ownership::Unmanaged);
}

Button MsgBox::add_header_button(const void* icon) {
  return Button(raw() ? lv_msgbox_add_header_button(raw(), icon) : nullptr,
                Ownership::Unmanaged);
}

Label MsgBox::add_text(const char* text) {
  return Label(raw() ? lv_msgbox_add_text(raw(), text) : nullptr,
               Ownership::Unmanaged);
}

Button MsgBox::add_footer_button(const char* text) {
  return Button(raw() ? lv_msgbox_add_footer_button(raw(), text) : nullptr,
                Ownership::Unmanaged);
}

Button MsgBox::add_close_button() {
  return Button(raw() ? lv_msgbox_add_close_button(raw()) : nullptr,
                Ownership::Unmanaged);
}

MsgBoxHeader MsgBox::get_header() {
  return MsgBoxHeader(raw() ? lv_msgbox_get_header(raw()) : nullptr,
                      Ownership::Unmanaged);
}

MsgBoxFooter MsgBox::get_footer() {
  return MsgBoxFooter(raw() ? lv_msgbox_get_footer(raw()) : nullptr,
                      Ownership::Unmanaged);
}

MsgBoxContent MsgBox::get_content() {
  return MsgBoxContent(raw() ? lv_msgbox_get_content(raw()) : nullptr,
                       Ownership::Unmanaged);
}

Label MsgBox::get_title() {
  return Label(raw() ? lv_msgbox_get_title(raw()) : nullptr,
               Ownership::Unmanaged);
}

void MsgBox::close() {
  if (raw()) {
    lv_msgbox_close(raw());
    // msgbox_close deletes the object instantly.
    // We must invalidate our wrapper to avoid double-free or use-after-free.
    obj_ = nullptr;
  }
}

void MsgBox::close_async() {
  if (raw()) {
    lv_msgbox_close_async(raw());
    // Async close will delete it later. The delete hook mechanism in Object
    // should handle the notification when LVGL eventually deletes it.
    // However, for safety, one might consider invalidating immediately if no
    // further usage is expected? No, standard practice is to rely on the hook
    // for async deletion.
  }
}

}  // namespace lvgl

#endif  // LV_USE_MSGBOX
