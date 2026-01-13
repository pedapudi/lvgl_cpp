#include "label.h"
#include "../core/observer.h"
#include <cstdarg>

namespace lvgl {

Label::Label() : Label((Object *)nullptr) {}

Label::Label(Object* parent)
    : Object(lv_label_create(parent ? parent->raw() : nullptr)) {}

Label::Label(lv_obj_t* obj) : Object(obj) {}

void Label::set_text(const std::string &text) {
  if (obj_)
    lv_label_set_text(obj_, text.c_str());
}

void Label::set_text(const char* text) {
  if (obj_)
    lv_label_set_text(obj_, text);
}

void Label::set_text_fmt(const char* fmt, ...) {
  if (!obj_)
    return;
  va_list args;
  va_start(args, fmt);
  lv_label_set_text_vfmt(obj_, fmt, args);
  va_end(args);
}

std::string Label::get_text() const {
  if (!obj_)
    return "";
  const char* txt = lv_label_get_text(obj_);
  return txt ? std::string(txt) : "";
}

void Label::set_long_mode(lv_label_long_mode_t mode) {
  if (obj_)
    lv_label_set_long_mode(obj_, mode);
}

lv_label_long_mode_t Label::get_long_mode() const {
  return obj_ ? lv_label_get_long_mode(obj_) : LV_LABEL_LONG_MODE_WRAP;
}

void Label::set_selection_start(uint32_t index) {
  if (obj_)
    lv_label_set_text_selection_start(obj_, index);
}

void Label::set_selection_end(uint32_t index) {
  if (obj_)
    lv_label_set_text_selection_end(obj_, index);
}

uint32_t Label::get_selection_start() const {
  return obj_ ? lv_label_get_text_selection_start(obj_)
              : LV_LABEL_TEXT_SELECTION_OFF;
}

uint32_t Label::get_selection_end() const {
  return obj_ ? lv_label_get_text_selection_end(obj_)
              : LV_LABEL_TEXT_SELECTION_OFF;
}

void Label::set_recolor(bool en) {
  if (obj_)
    lv_label_set_recolor(obj_, en);
}

bool Label::get_recolor() const {
  return obj_ ? lv_label_get_recolor(obj_) : false;
}

Observer Label::bind_text(Subject& subject, const char* fmt) {
  return Observer(lv_label_bind_text(raw(), subject.raw(), fmt));
}

} // namespace lvgl
