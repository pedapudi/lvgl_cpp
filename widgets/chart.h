#ifndef LVGL_CPP_WIDGETS_CHART_H_
#define LVGL_CPP_WIDGETS_CHART_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
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

class Chart;  // Forward declaration

/**
 * @brief Wrapper for a chart series.
 */
class ChartSeries {
 public:
  ChartSeries(Chart* chart, lv_chart_series_t* series)
      : chart_(chart), series_(series) {}

  void set_next_value(int32_t value);
  void set_next_value2(int32_t x_value, int32_t y_value);
  void set_all_values(int32_t value);
  void set_color(lv_color_t color);
  void set_value_by_id(uint32_t id, int32_t value);

  lv_chart_series_t* raw() const { return series_; }

 private:
  Chart* chart_;
  lv_chart_series_t* series_;
};

class Chart : public Widget<Chart> {
 public:
  Chart();
  explicit Chart(Object* parent, Ownership ownership = Ownership::Default);
  explicit Chart(Object& parent);
  explicit Chart(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Chart& set_type(lv_chart_type_t type);
  Chart& set_type(ChartType type) {
    return set_type(static_cast<lv_chart_type_t>(type));
  }
  Chart& set_point_count(uint32_t cnt);
  Chart& set_axis_range(lv_chart_axis_t axis, int32_t min, int32_t max);
  Chart& set_div_line_count(uint32_t hdiv, uint32_t vdiv);
  Chart& set_update_mode(lv_chart_update_mode_t update_mode);
  Chart& set_update_mode(ChartUpdateMode update_mode) {
    return set_update_mode(static_cast<lv_chart_update_mode_t>(update_mode));
  }

  lv_chart_type_t get_type();
  uint32_t get_point_count();

  ChartSeries add_series(lv_color_t color, lv_chart_axis_t axis);
  void remove_series(ChartSeries series);
  // Removed specific set_series_* methods as they are now in ChartSeries
  void refresh();
};

}  // namespace lvgl

#endif  // LV_USE_CHART
#endif  // LVGL_CPP_WIDGETS_CHART_H_
