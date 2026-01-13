#ifndef LVGL_CPP_WIDGETS_SPAN_H_
#define LVGL_CPP_WIDGETS_SPAN_H_

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

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

class SpanGroup : public Object {
 public:
  /**
   * @brief Create a SpanGroup on the active screen.
   */
  SpanGroup();
  /**
   * @brief Create a SpanGroup with a parent.
   * @param parent The parent object.
   */
  explicit SpanGroup(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit SpanGroup(lv_obj_t* obj);

  lv_span_t* add_span();
  void delete_span(lv_span_t* span);
  void set_span_text(lv_span_t* span, const char* text);
  void set_span_style(lv_span_t* span, const lv_style_t* style);
  void set_align(lv_text_align_t align);
  void set_overflow(lv_span_overflow_t overflow);
  void set_indent(int32_t indent);
  void set_mode(lv_span_mode_t mode);
  void set_max_lines(int32_t lines);

  lv_span_t* get_child(int32_t id);
  uint32_t get_span_count();
  lv_text_align_t get_align();
  lv_span_overflow_t get_overflow();
  int32_t get_indent();
  lv_span_mode_t get_mode();
  int32_t get_max_lines();
  void refresh();
};

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
}  // namespace lvgl

#endif  // LVGL_CPP_WIDGETS_SPAN_H_
