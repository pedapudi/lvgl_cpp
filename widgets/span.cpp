#include "span.h"

#if LV_USE_SPAN

namespace lvgl {

SpanGroup::SpanGroup() : SpanGroup((Object*)nullptr) {}

SpanGroup::SpanGroup(Object* parent, Ownership ownership)
    : Object(lv_spangroup_create(parent ? parent->raw() : nullptr), ownership) {
}

SpanGroup::SpanGroup(lv_obj_t* obj, Ownership ownership)
    : Object(obj, ownership) {}

lv_span_t* SpanGroup::add_span() {
  return obj_ ? lv_spangroup_add_span(obj_) : nullptr;
}

void SpanGroup::delete_span(lv_span_t* span) {
  if (obj_) lv_spangroup_delete_span(obj_, span);
}

SpanGroup& SpanGroup::set_span_text(lv_span_t* span, const char* text) {
  if (obj_) lv_spangroup_set_span_text(obj_, span, text);
  return *this;
}

SpanGroup& SpanGroup::set_span_style(lv_span_t* span, const lv_style_t* style) {
  if (obj_) lv_spangroup_set_span_style(obj_, span, style);
  return *this;
}

SpanGroup& SpanGroup::set_align(lv_text_align_t align) {
  if (obj_) lv_spangroup_set_align(obj_, align);
  return *this;
}

SpanGroup& SpanGroup::set_overflow(lv_span_overflow_t overflow) {
  if (obj_) lv_spangroup_set_overflow(obj_, overflow);
  return *this;
}

SpanGroup& SpanGroup::set_indent(int32_t indent) {
  if (obj_) lv_spangroup_set_indent(obj_, indent);
  return *this;
}

SpanGroup& SpanGroup::set_mode(lv_span_mode_t mode) {
  if (obj_) lv_spangroup_set_mode(obj_, mode);
  return *this;
}

SpanGroup& SpanGroup::set_max_lines(int32_t lines) {
  if (obj_) lv_spangroup_set_max_lines(obj_, lines);
  return *this;
}

SpanGroup& SpanGroup::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
SpanGroup& SpanGroup::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
SpanGroup& SpanGroup::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
SpanGroup& SpanGroup::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
SpanGroup& SpanGroup::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
SpanGroup& SpanGroup::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
SpanGroup& SpanGroup::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
SpanGroup& SpanGroup::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

lv_span_t* SpanGroup::get_child(int32_t id) {
  return obj_ ? lv_spangroup_get_child(obj_, id) : nullptr;
}

uint32_t SpanGroup::get_span_count() {
  return obj_ ? lv_spangroup_get_span_count(obj_) : 0;
}

lv_text_align_t SpanGroup::get_align() {
  return obj_ ? lv_spangroup_get_align(obj_) : LV_TEXT_ALIGN_LEFT;
}

lv_span_overflow_t SpanGroup::get_overflow() {
  return obj_ ? lv_spangroup_get_overflow(obj_) : LV_SPAN_OVERFLOW_CLIP;
}

int32_t SpanGroup::get_indent() {
  return obj_ ? lv_spangroup_get_indent(obj_) : 0;
}

lv_span_mode_t SpanGroup::get_mode() {
  return obj_ ? lv_spangroup_get_mode(obj_) : LV_SPAN_MODE_FIXED;
}

int32_t SpanGroup::get_max_lines() {
  return obj_ ? lv_spangroup_get_max_lines(obj_) : -1;
}

void SpanGroup::refresh() {
  if (obj_) lv_spangroup_refresh(obj_);
}

}  // namespace lvgl

#endif  // LV_USE_SPAN
