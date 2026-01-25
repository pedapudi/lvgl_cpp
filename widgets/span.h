#ifndef LVGL_CPP_WIDGETS_SPAN_H_
#define LVGL_CPP_WIDGETS_SPAN_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_SPAN
/**
 * @file span.h
 * @brief C++ Wrapper for LVGL SpanGroup Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::SpanGroup widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class SpanGroup;

class Span {
 public:
  Span(lv_span_t* span, SpanGroup* group);

  Span& set_text(const char* text);
  Span& set_style(const Style& style);
  Span& style(const Style& style);  // Alias

  lv_span_t* raw() const;

 private:
  lv_span_t* span_;
  SpanGroup* group_;
};

class SpanGroup : public Widget<SpanGroup> {
 public:
  SpanGroup();
  explicit SpanGroup(Object* parent, Ownership ownership = Ownership::Default);
  explicit SpanGroup(Object& parent);
  explicit SpanGroup(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Span add_span();

  // Legacy/Raw access
  lv_span_t* add_span_raw();
  void delete_span(Span& span);
  void delete_span_raw(lv_span_t* span);

  SpanGroup& set_align(TextAlign align);
  SpanGroup& set_overflow(lv_span_overflow_t overflow);
  SpanGroup& set_indent(int32_t indent);
  SpanGroup& set_mode(lv_span_mode_t mode);  // TODO: Enum
  SpanGroup& set_max_lines(int32_t lines);

  Span get_child(int32_t id);
  uint32_t get_span_count();
  TextAlign get_align();
  lv_span_overflow_t get_overflow();
  int32_t get_indent();
  lv_span_mode_t get_mode();
  int32_t get_max_lines();
  void refresh();
};

}  // namespace lvgl

#endif  // LV_USE_SPAN
#endif  // LVGL_CPP_WIDGETS_SPAN_H_
