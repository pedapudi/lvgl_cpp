#ifndef LVGL_CPP_WIDGETS_LINE_H_
#define LVGL_CPP_WIDGETS_LINE_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_LINE
/**
 * @file line.h
 * @brief C++ Wrapper for LVGL Line Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Line widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Line : public Object {
 public:
  /**
   * @brief Create a Line on the active screen.
   */
  Line();
  /**
   * @brief Create a Line with a parent.
   * @param parent The parent object.
   */
  explicit Line(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Line(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Line& set_points(const lv_point_precise_t points[], uint32_t point_num);
  Line& set_points_mutable(lv_point_precise_t points[], uint32_t point_num);
  Line& set_y_invert(bool en);

  // Fluent API shadows
  Line& set_width(int32_t width);
  Line& set_height(int32_t height);
  Line& set_size(int32_t width, int32_t height);
  Line& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Line& add_state(lv_state_t state);
  Line& remove_state(lv_state_t state);
  Line& add_flag(lv_obj_flag_t flag);
  Line& remove_flag(lv_obj_flag_t flag);

  const lv_point_precise_t* get_points() const;
  uint32_t get_point_count() const;
  bool is_point_array_mutable() const;
  lv_point_precise_t* get_points_mutable();
  bool get_y_invert() const;
};

/**
 * @file line.h
 * @brief C++ Wrapper for LVGL Line Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Line widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl

#endif  // LV_USE_LINE
#endif  // LVGL_CPP_WIDGETS_LINE_H_
