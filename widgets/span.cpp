#include "span.h"

#if LV_USE_SPAN

namespace lvgl {

SpanGroup::SpanGroup()
    : SpanGroup(static_cast<Object*>(nullptr), Ownership::Managed) {}

SpanGroup::SpanGroup(Object* parent, Ownership ownership)
    : Widget(lv_spangroup_create(parent ? parent->raw() : nullptr), ownership) {
}

SpanGroup::SpanGroup(Object& parent) : SpanGroup(&parent) {}

SpanGroup::SpanGroup(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

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
