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
  if (raw()) lv_label_set_text(raw(), text.c_str());
  return *this;
}

Label& Label::set_text(const char* text) {
  if (raw()) lv_label_set_text(raw(), text);
  return *this;
}

Label& Label::set_text_fmt(const char* fmt, ...) {
  if (!raw()) return *this;
  va_list args;
  va_start(args, fmt);
  set_text_vfmt(fmt, args);
  va_end(args);
  return *this;
}

Label& Label::set_text_vfmt(const char* fmt, va_list args) {
  if (raw()) lv_label_set_text_vfmt(raw(), fmt, args);
  return *this;
}

std::string Label::get_text() const {
  if (!raw()) return "";
  const char* txt = lv_label_get_text(raw());
  return txt ? std::string(txt) : "";
}

Label& Label::set_long_mode(LongMode mode) {
  if (raw())
    lv_label_set_long_mode(raw(), static_cast<lv_label_long_mode_t>(mode));
  return *this;
}

Label::LongMode Label::get_long_mode() const {
  return raw() ? static_cast<LongMode>(lv_label_get_long_mode(raw()))
              : LongMode::Wrap;
}

Label& Label::set_selection_start(uint32_t index) {
  if (raw()) lv_label_set_text_selection_start(raw(), index);
  return *this;
}

Label& Label::set_selection_end(uint32_t index) {
  if (raw()) lv_label_set_text_selection_end(raw(), index);
  return *this;
}

uint32_t Label::get_selection_start() const {
  return raw() ? lv_label_get_text_selection_start(raw())
              : LV_LABEL_TEXT_SELECTION_OFF;
}

uint32_t Label::get_selection_end() const {
  return raw() ? lv_label_get_text_selection_end(raw())
              : LV_LABEL_TEXT_SELECTION_OFF;
}

Label& Label::set_recolor(bool en) {
  if (raw()) lv_label_set_recolor(raw(), en);
  return *this;
}

Label& Label::ins_text(uint32_t pos, const char* txt) {
  if (raw()) lv_label_ins_text(raw(), pos, txt);
  return *this;
}

Label& Label::cut_text(uint32_t pos, uint32_t cnt) {
  if (raw()) lv_label_cut_text(raw(), pos, cnt);
  return *this;
}

bool Label::get_recolor() const {
  return raw() ? lv_label_get_recolor(raw()) : false;
}

uint32_t Label::get_letter_on(const Point& point_in) const {
  return raw() ? lv_label_get_letter_on(
                    raw(), const_cast<lv_point_t*>(point_in.raw()), false)
              : LV_LABEL_TEXT_SELECTION_OFF;
}

bool Label::is_char_under_pos(const Point& pos) const {
  return raw() ? lv_label_is_char_under_pos(raw(),
                                           const_cast<lv_point_t*>(pos.raw()))
              : false;
}

Point Label::get_letter_pos(uint32_t char_id) const {
  lv_point_t p = {0, 0};
  if (raw()) lv_label_get_letter_pos(raw(), char_id, &p);
  return Point(p);
}

Observer Label::bind_text(Subject& subject, const char* fmt) {
  return Observer(lv_label_bind_text(raw(), subject.raw(), fmt), false);
}

Label& Label::set_text_static(const char* text) {
  if (raw()) lv_label_set_text_static(raw(), text);
  return *this;
}

Label& Label::set_translation_tag(const char* txt) {
#if LV_USE_I18N
  if (raw()) lv_label_set_translation_tag(raw(), txt);
#else
  (void)txt;
#endif
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_LABEL
