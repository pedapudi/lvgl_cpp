#ifndef LVGL_CPP_WIDGETS_CHART_H_
#define LVGL_CPP_WIDGETS_CHART_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export


#if LV_USE_CHART
/**
 * @file chart.h
 * @brief C++ Wrapper for LVGL Chart Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Chart widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Chart : public Object {
 public:
  /**
   * @brief Create a Chart on the active screen.
   */
  Chart();
  /**
   * @brief Create a Chart with a parent.
   * @param parent The parent object.
   */
  explicit Chart(Object* parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Chart(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  void set_type(lv_chart_type_t type);
  void set_point_count(uint32_t cnt);
  void set_axis_range(lv_chart_axis_t axis, int32_t min, int32_t max);
  void set_div_line_count(uint32_t hdiv, uint32_t vdiv);
  void set_update_mode(lv_chart_update_mode_t update_mode);

  lv_chart_type_t get_type();
  uint32_t get_point_count();

  lv_chart_series_t* add_series(lv_color_t color, lv_chart_axis_t axis);
  void remove_series(lv_chart_series_t* series);
  void set_series_color(lv_chart_series_t* series, lv_color_t color);
  void set_next_value(lv_chart_series_t* series, int32_t value);
  void set_next_value2(lv_chart_series_t* series, int32_t x_value,
                       int32_t y_value);
  void set_all_values(lv_chart_series_t* series, int32_t value);
  void refresh();
};

/**
 * @file chart.h
 * @brief C++ Wrapper for LVGL Chart Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Chart widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl


#endif // LV_USE_CHART
#endif  // LVGL_CPP_WIDGETS_CHART_H_
