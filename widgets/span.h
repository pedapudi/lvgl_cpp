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
  Span& set_text_static(const char* text);
  Span& set_text_fmt(const char* fmt, ...);
  Span& set_style(const Style& style);
  Span& style(const Style& style);  // Alias

  const char* get_text() const;
  lv_span_t* raw() const;
  void refresh();

 private:
  lv_span_t* span_;
  SpanGroup* group_;
};

class SpanGroup : public Widget<SpanGroup> {
 public:
  /**
   * @brief Span group display modes.
   */
  enum class Mode : uint8_t {
    Fixed = LV_SPAN_MODE_FIXED,
    Expand = LV_SPAN_MODE_EXPAND,
    Break = LV_SPAN_MODE_BREAK,
  };

  /**
   * @brief Span group overflow modes.
   */
  enum class Overflow : uint8_t {
    Clip = LV_SPAN_OVERFLOW_CLIP,
    Ellipsis = LV_SPAN_OVERFLOW_ELLIPSIS,
  };

  SpanGroup();
  explicit SpanGroup(Object* parent, Ownership ownership = Ownership::Default);
  explicit SpanGroup(Object& parent);
  explicit SpanGroup(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Span add_span();
  void delete_span(Span& span);

  SpanGroup& set_align(TextAlign align);
  SpanGroup& set_overflow(Overflow overflow);
  SpanGroup& set_indent(int32_t indent);
  SpanGroup& set_mode(Mode mode);
  SpanGroup& set_max_lines(int32_t lines);

  Span get_child(int32_t id);
  uint32_t get_span_count() const;
  TextAlign get_align() const;
  Overflow get_overflow() const;
  int32_t get_indent() const;
  Mode get_mode() const;
  int32_t get_max_lines() const;
  int32_t get_max_line_height() const;

  void refresh();
};

}  // namespace lvgl

#endif  // LV_USE_SPAN
#endif  // LVGL_CPP_WIDGETS_SPAN_H_
