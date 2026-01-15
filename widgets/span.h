#ifndef LVGL_CPP_WIDGETS_SPAN_H_
#define LVGL_CPP_WIDGETS_SPAN_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
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
  explicit SpanGroup(Object* parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit SpanGroup(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  lv_span_t* add_span();
  void delete_span(lv_span_t* span);
  SpanGroup& set_span_text(lv_span_t* span, const char* text);
  SpanGroup& set_span_style(lv_span_t* span, const lv_style_t* style);
  SpanGroup& set_align(lv_text_align_t align);
  SpanGroup& set_overflow(lv_span_overflow_t overflow);
  SpanGroup& set_indent(int32_t indent);
  SpanGroup& set_mode(lv_span_mode_t mode);
  SpanGroup& set_max_lines(int32_t lines);

  // Fluent API shadows
  SpanGroup& set_width(int32_t width);
  SpanGroup& set_height(int32_t height);
  SpanGroup& set_size(int32_t width, int32_t height);
  SpanGroup& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  SpanGroup& add_state(lv_state_t state);
  SpanGroup& remove_state(lv_state_t state);
  SpanGroup& add_flag(lv_obj_flag_t flag);
  SpanGroup& remove_flag(lv_obj_flag_t flag);

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

#endif  // LV_USE_SPAN
#endif  // LVGL_CPP_WIDGETS_SPAN_H_
