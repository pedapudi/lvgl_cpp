#include "span.h"

#if LV_USE_SPAN
#include "../misc/style.h"

namespace lvgl {

// ============================================================================
// Span Proxy Implementation
// ============================================================================

Span::Span(lv_span_t* span, SpanGroup* group) : span_(span), group_(group) {}

Span& Span::set_text(const char* text) {
  if (span_) lv_span_set_text(span_, text);
  return *this;
}

Span& Span::set_text_static(const char* text) {
  if (span_) lv_span_set_text_static(span_, text);
  return *this;
}

Span& Span::set_text_fmt(const char* fmt, ...) {
  if (span_) {
    va_list args;
    va_start(args, fmt);
    char buf[256];  // Temporary buffer for formatting
    vsnprintf(buf, sizeof(buf), fmt, args);
    lv_span_set_text(span_, buf);
    va_end(args);
  }
  return *this;
}

Span& Span::set_style(const Style& style) {
  if (group_->raw() && span_)
    lv_spangroup_set_span_style(group_->raw(), span_, style.raw());
  return *this;
}

Span& Span::style(const Style& style) { return set_style(style); }

const char* Span::get_text() const {
  return span_ ? lv_span_get_text(span_) : nullptr;
}

lv_span_t* Span::raw() const { return span_; }

void Span::refresh() {
  if (group_) group_->refresh();
}

// ============================================================================
// SpanGroup Implementation
// ============================================================================

SpanGroup::SpanGroup()
    : SpanGroup(static_cast<Object*>(nullptr), Ownership::Managed) {}

SpanGroup::SpanGroup(Object* parent, Ownership ownership)
    : Widget(lv_spangroup_create(parent ? parent->raw() : nullptr), ownership) {
}

SpanGroup::SpanGroup(Object& parent) : SpanGroup(&parent) {}

SpanGroup::SpanGroup(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

Span SpanGroup::add_span() {
  lv_span_t* span = obj_ ? lv_spangroup_add_span(obj_) : nullptr;
  return Span(span, this);
}

void SpanGroup::delete_span(Span& span) {
  if (obj_) lv_spangroup_delete_span(obj_, span.raw());
}

SpanGroup& SpanGroup::set_align(TextAlign align) {
  if (obj_) lv_spangroup_set_align(obj_, static_cast<lv_text_align_t>(align));
  return *this;
}

SpanGroup& SpanGroup::set_overflow(lv_span_overflow_t overflow) {
  if (obj_) lv_spangroup_set_overflow(obj_, overflow);
  return *this;
}

SpanGroup& SpanGroup::set_overflow(Overflow overflow) {
  return set_overflow(static_cast<lv_span_overflow_t>(overflow));
}

SpanGroup& SpanGroup::set_indent(int32_t indent) {
  if (obj_) lv_spangroup_set_indent(obj_, indent);
  return *this;
}

SpanGroup& SpanGroup::set_mode(lv_span_mode_t mode) {
  if (obj_) lv_spangroup_set_mode(obj_, mode);
  return *this;
}

SpanGroup& SpanGroup::set_mode(Mode mode) {
  return set_mode(static_cast<lv_span_mode_t>(mode));
}

SpanGroup& SpanGroup::set_max_lines(int32_t lines) {
  if (obj_) lv_spangroup_set_max_lines(obj_, lines);
  return *this;
}

Span SpanGroup::get_child(int32_t id) {
  lv_span_t* span = obj_ ? lv_spangroup_get_child(obj_, id) : nullptr;
  return Span(span, this);
}

uint32_t SpanGroup::get_span_count() const {
  return obj_ ? lv_spangroup_get_span_count(obj_) : 0;
}

TextAlign SpanGroup::get_align() const {
  return obj_ ? static_cast<TextAlign>(lv_spangroup_get_align(obj_))
              : TextAlign::Left;
}

SpanGroup::Overflow SpanGroup::get_overflow() const {
  return obj_ ? static_cast<Overflow>(lv_spangroup_get_overflow(obj_))
              : Overflow::Clip;
}

int32_t SpanGroup::get_indent() const {
  return obj_ ? lv_spangroup_get_indent(obj_) : 0;
}

SpanGroup::Mode SpanGroup::get_mode() const {
  return obj_ ? static_cast<Mode>(lv_spangroup_get_mode(obj_)) : Mode::Fixed;
}

int32_t SpanGroup::get_max_lines() const {
  return obj_ ? lv_spangroup_get_max_lines(obj_) : -1;
}

int32_t SpanGroup::get_max_line_height() const {
  return obj_ ? lv_spangroup_get_max_line_height(obj_) : 0;
}

void SpanGroup::refresh() {
  if (obj_) lv_spangroup_refresh(obj_);
}

}  // namespace lvgl

#endif  // LV_USE_SPAN
