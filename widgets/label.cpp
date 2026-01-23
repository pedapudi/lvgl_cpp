#include "label.h"

#include <cstdarg>

#include "../core/observer.h"

#if LV_USE_LABEL

namespace lvgl {

Label::Label() : Label(static_cast<Object*>(nullptr), Ownership::Managed) {}

Label::Label(Object* parent, Ownership ownership)
    : Widget(lv_label_create(parent ? parent->raw() : nullptr), ownership) {}

Label::Label(Object& parent) : Label(&parent) {}

Label::Label(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Label::Label(Object& parent, const std::string& text) : Label(&parent) {
  set_text(text);
}

Label::Label(Object& parent, const char* text) : Label(&parent) {
  set_text(text);
}

Label& Label::set_text(const std::string& text) {
  if (obj_) lv_label_set_text(obj_, text.c_str());
  return *this;
}

Label& Label::set_text(const char* text) {
  if (obj_) lv_label_set_text(obj_, text);
  return *this;
}

Label& Label::set_text_fmt(const char* fmt, ...) {
  if (!obj_) return *this;
  va_list args;
  va_start(args, fmt);
  lv_label_set_text_vfmt(obj_, fmt, args);
  va_end(args);
  return *this;
}

std::string Label::get_text() const {
  if (!obj_) return "";
  const char* txt = lv_label_get_text(obj_);
  return txt ? std::string(txt) : "";
}

Label& Label::set_long_mode(LongMode mode) {
  if (obj_)
    lv_label_set_long_mode(obj_, static_cast<lv_label_long_mode_t>(mode));
  return *this;
}

Label::LongMode Label::get_long_mode() const {
  return obj_ ? static_cast<LongMode>(lv_label_get_long_mode(obj_))
              : LongMode::Wrap;
}

Label& Label::set_selection_start(uint32_t index) {
  if (obj_) lv_label_set_text_selection_start(obj_, index);
  return *this;
}

Label& Label::set_selection_end(uint32_t index) {
  if (obj_) lv_label_set_text_selection_end(obj_, index);
  return *this;
}

uint32_t Label::get_selection_start() const {
  return obj_ ? lv_label_get_text_selection_start(obj_)
              : LV_LABEL_TEXT_SELECTION_OFF;
}

uint32_t Label::get_selection_end() const {
  return obj_ ? lv_label_get_text_selection_end(obj_)
              : LV_LABEL_TEXT_SELECTION_OFF;
}

Label& Label::set_recolor(bool en) {
  if (obj_) lv_label_set_recolor(obj_, en);
  return *this;
}

bool Label::get_recolor() const {
  return obj_ ? lv_label_get_recolor(obj_) : false;
}

Observer Label::bind_text(Subject& subject, const char* fmt) {
  return Observer(lv_label_bind_text(raw(), subject.raw(), fmt));
}

}  // namespace lvgl

#endif  // LV_USE_LABEL
