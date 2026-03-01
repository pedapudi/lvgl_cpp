#include "arclabel.h"

#include <cstdarg>

#if LV_USE_ARCLABEL

namespace lvgl {

ArcLabel::ArcLabel()
    : ArcLabel(static_cast<Object*>(nullptr), Ownership::Managed) {}

ArcLabel::ArcLabel(Object* parent, Ownership ownership)
    : Widget(lv_arclabel_create(parent ? parent->raw() : nullptr), ownership) {}

ArcLabel::ArcLabel(Object& parent) : ArcLabel(&parent) {}

ArcLabel::ArcLabel(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

ArcLabel& ArcLabel::set_text(const std::string& text) {
  if (raw()) lv_arclabel_set_text(raw(), text.c_str());
  return *this;
}

ArcLabel& ArcLabel::set_text(const char* text) {
  if (raw()) lv_arclabel_set_text(raw(), text);
  return *this;
}

ArcLabel& ArcLabel::set_text_fmt(const char* fmt, ...) {
  if (!raw()) return *this;
  va_list args;
  va_start(args, fmt);
  lv_arclabel_set_text_vfmt(raw(), fmt, args);
  va_end(args);
  return *this;
}

ArcLabel& ArcLabel::set_text_static(const char* text) {
  if (raw()) lv_arclabel_set_text_static(raw(), text);
  return *this;
}

const char* ArcLabel::get_text() const {
  return raw() ? lv_arclabel_get_text(raw()) : "";
}

ArcLabel& ArcLabel::set_angle_start(lv_value_precise_t start) {
  if (raw()) lv_arclabel_set_angle_start(raw(), start);
  return *this;
}

lv_value_precise_t ArcLabel::get_angle_start() const {
  return raw() ? lv_arclabel_get_angle_start(raw()) : 0;
}

ArcLabel& ArcLabel::set_angle_size(lv_value_precise_t size) {
  if (raw()) lv_arclabel_set_angle_size(raw(), size);
  return *this;
}

lv_value_precise_t ArcLabel::get_angle_size() const {
  return raw() ? lv_arclabel_get_angle_size(raw()) : 0;
}

ArcLabel& ArcLabel::set_offset(int32_t offset) {
  if (raw()) lv_arclabel_set_offset(raw(), offset);
  return *this;
}

ArcLabel& ArcLabel::set_dir(Direction dir) {
  if (raw()) lv_arclabel_set_dir(raw(), static_cast<lv_arclabel_dir_t>(dir));
  return *this;
}

ArcLabel::Direction ArcLabel::get_dir() const {
  return raw() ? static_cast<Direction>(lv_arclabel_get_dir(raw()))
               : Direction::Clockwise;
}

ArcLabel& ArcLabel::set_radius(uint32_t radius) {
  if (raw()) lv_arclabel_set_radius(raw(), radius);
  return *this;
}

uint32_t ArcLabel::get_radius() const {
  return raw() ? lv_arclabel_get_radius(raw()) : 0;
}

ArcLabel& ArcLabel::set_center_offset_x(uint32_t x) {
  if (raw()) lv_arclabel_set_center_offset_x(raw(), x);
  return *this;
}

ArcLabel& ArcLabel::set_center_offset_y(uint32_t y) {
  if (raw()) lv_arclabel_set_center_offset_y(raw(), y);
  return *this;
}

uint32_t ArcLabel::get_center_offset_x() const {
  return raw() ? lv_arclabel_get_center_offset_x(raw()) : 0;
}

uint32_t ArcLabel::get_center_offset_y() const {
  return raw() ? lv_arclabel_get_center_offset_y(raw()) : 0;
}

ArcLabel& ArcLabel::set_text_vertical_align(TextAlign align) {
  if (raw())
    lv_arclabel_set_text_vertical_align(
        raw(), static_cast<lv_arclabel_text_align_t>(align));
  return *this;
}

ArcLabel& ArcLabel::set_text_horizontal_align(TextAlign align) {
  if (raw())
    lv_arclabel_set_text_horizontal_align(
        raw(), static_cast<lv_arclabel_text_align_t>(align));
  return *this;
}

ArcLabel::TextAlign ArcLabel::get_text_vertical_align() const {
  return raw() ? static_cast<TextAlign>(
                     lv_arclabel_get_text_vertical_align(raw()))
               : TextAlign::Default;
}

ArcLabel::TextAlign ArcLabel::get_text_horizontal_align() const {
  return raw() ? static_cast<TextAlign>(
                     lv_arclabel_get_text_horizontal_align(raw()))
               : TextAlign::Default;
}

ArcLabel& ArcLabel::set_overflow(Overflow overflow) {
  if (raw())
    lv_arclabel_set_overflow(raw(),
                             static_cast<lv_arclabel_overflow_t>(overflow));
  return *this;
}

ArcLabel::Overflow ArcLabel::get_overflow() const {
  return raw() ? static_cast<Overflow>(lv_arclabel_get_overflow(raw()))
               : Overflow::Visible;
}

ArcLabel& ArcLabel::set_end_overlap(bool overlap) {
  if (raw()) lv_arclabel_set_end_overlap(raw(), overlap);
  return *this;
}

bool ArcLabel::get_end_overlap() const {
  return raw() ? lv_arclabel_get_end_overlap(raw()) : false;
}

ArcLabel& ArcLabel::set_recolor(bool en) {
  if (raw()) lv_arclabel_set_recolor(raw(), en);
  return *this;
}

bool ArcLabel::get_recolor() const {
  return raw() ? lv_arclabel_get_recolor(raw()) : false;
}

lv_value_precise_t ArcLabel::get_text_angle() const {
  return raw() ? lv_arclabel_get_text_angle(raw()) : 0;
}

}  // namespace lvgl

#endif  // LV_USE_ARCLABEL
