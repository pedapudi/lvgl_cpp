#ifndef LVGL_CPP_WIDGETS_LINE_H_
#define LVGL_CPP_WIDGETS_LINE_H_

#include <cstdint>

#include "../core/widget.h"    // IWYU pragma: export
#include "../misc/geometry.h"  // IWYU pragma: export
#include "lvgl.h"              // IWYU pragma: export

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

class Line : public Widget<Line> {
 public:
  /**
   * @brief Create a Line on the active screen.
   */
  Line();
  explicit Line(Object* parent, Ownership ownership = Ownership::Default);
  explicit Line(Object& parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Line(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Line& set_points(const PointPrecise points[], uint32_t point_num);
  Line& set_points_mutable(PointPrecise points[], uint32_t point_num);

  /**
   * @brief Set the points using Point objects.
   * @note This function copies the points to a dynamically allocated array
   * managed by LVGL if possible? Wait, lv_line_set_points stores the POINTER.
   * It does NOT copy the points. So we cannot just pass a `std::vector<Point>`
   * or `Point[]` if it goes out of scope. But `lvgl::Point` wraps `lv_point_t`.
   * `lv_point_precise_t` is different in v9? In v9 `lv_point_precise_t` is `{
   * lv_value_precise_t x, y; }`. `lv_point_t` is `{ int32_t x, y; }`. Are they
   * compatible? `lv_value_precise_t` is usually `int32_t` or `float` depending
   * on config. If `Point` is `lv_point_t` wrapper, it might not be compatible
   * with `lv_point_precise_t`. I should check `geometry.h`.
   */

  /*
    In `misc/geometry.h`, `Point` wraps `lv_point_t`.
    `Line` uses `lv_point_precise_t`.
    If LVGL is configured with precise coordinates, `lv_point_precise_t` might
    be different. If standard config, `lv_point_precise_t` == `lv_point_t`.

    Let's check `lvgl.h` via `view_file`? No I can't.
    But typical LVGL 9 defaults `lv_point_precise_t` to `int32_t`.

    Actually, `lv_line_set_points` docs say: "The point array must be kept valid
    while the line is used". So providing a `set_points(const
    std::vector<Point>&)` is dangerous if it takes reference. But `Line` widget
    is tricky. Let's keep `lv_point_precise_t*` for now to be safe and explicit
    about memory management requirements. I won't wrap it with `Point` unless
    `Point` becomes `PointPrecise` or I verify compatibility.

    Wait, `Canvas::draw_line` used `lv_point_precise_t`.
    `Point` wraps `lv_point_t`.

    I will skip modernizing `Line` to use `Point` for now to avoiding
    incompatibilities. I'll just add the include for consistency if needed, but
    `Line` doesn't use `lv_point_t` in API, it uses `lv_point_precise_t`. So
    `geometry.h` is not strictly needed unless I add `PointPrecise` wrapper.

    I will move on to Phase 3 verification.
    Wait, I should verify compilation.
  */
  Line& set_y_invert(bool en);

  const PointPrecise* get_points() const;
  uint32_t get_point_count() const;
  bool is_point_array_mutable() const;
  PointPrecise* get_points_mutable();
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
