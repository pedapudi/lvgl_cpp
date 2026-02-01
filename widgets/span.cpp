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
  lv_span_t* span = raw() ? lv_spangroup_add_span(raw()) : nullptr;
  return Span(span, this);
}

void SpanGroup::delete_span(Span& span) {
  if (raw()) lv_spangroup_delete_span(raw(), span.raw());
}

SpanGroup& SpanGroup::set_align(TextAlign align) {
  if (raw()) lv_spangroup_set_align(raw(), static_cast<lv_text_align_t>(align));
  return *this;
}

SpanGroup& SpanGroup::set_overflow(Overflow overflow) {
  if (raw())
    lv_spangroup_set_overflow(raw(), static_cast<lv_span_overflow_t>(overflow));
  return *this;
}

SpanGroup& SpanGroup::set_indent(int32_t indent) {
  if (raw()) lv_spangroup_set_indent(raw(), indent);
  return *this;
}

SpanGroup& SpanGroup::set_mode(Mode mode) {
  if (raw()) lv_spangroup_set_mode(raw(), static_cast<lv_span_mode_t>(mode));
  return *this;
}

SpanGroup& SpanGroup::set_max_lines(int32_t lines) {
  if (raw()) lv_spangroup_set_max_lines(raw(), lines);
  return *this;
}

Span SpanGroup::get_child(int32_t id) {
  lv_span_t* span = raw() ? lv_spangroup_get_child(raw(), id) : nullptr;
  return Span(span, this);
}

uint32_t SpanGroup::get_span_count() const {
  return raw() ? lv_spangroup_get_span_count(raw()) : 0;
}

TextAlign SpanGroup::get_align() const {
  return raw() ? static_cast<TextAlign>(lv_spangroup_get_align(raw()))
               : TextAlign::Left;
}

SpanGroup::Overflow SpanGroup::get_overflow() const {
  return raw() ? static_cast<Overflow>(lv_spangroup_get_overflow(raw()))
               : Overflow::Clip;
}

int32_t SpanGroup::get_indent() const {
  return raw() ? lv_spangroup_get_indent(raw()) : 0;
}

SpanGroup::Mode SpanGroup::get_mode() const {
  return raw() ? static_cast<Mode>(lv_spangroup_get_mode(raw())) : Mode::Fixed;
}

int32_t SpanGroup::get_max_lines() const {
  return raw() ? lv_spangroup_get_max_lines(raw()) : -1;
}

int32_t SpanGroup::get_max_line_height() const {
  return raw() ? lv_spangroup_get_max_line_height(raw()) : 0;
}

void SpanGroup::refresh() {
  if (raw()) lv_spangroup_refresh(raw());
}

}  // namespace lvgl

#endif  // LV_USE_SPAN
