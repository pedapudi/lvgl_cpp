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

/**
 * @brief Wrapper for a chart cursor.
 */
class ChartCursor {
 public:
  ChartCursor(Chart* chart, lv_chart_cursor_t* cursor)
      : chart_(chart), cursor_(cursor) {}

  /**
   * @brief Set the cursor position using a point.
   */
  void set_pos(lv_point_t pos);

  /**
   * @brief Set the cursor X position.
   */
  void set_pos_x(int32_t x);

  /**
   * @brief Set the cursor Y position.
   */
  void set_pos_y(int32_t y);

  /**
   * @brief Set the cursor to a specific point on a series.
   */
  void set_point(ChartSeries& series, uint32_t point_id);

  /**
   * @brief Get the current cursor point position.
   */
  lv_point_t get_point() const;

  lv_chart_cursor_t* raw() const { return cursor_; }

 private:
  Chart* chart_;
  lv_chart_cursor_t* cursor_;
};

class Chart : public Widget<Chart> {
 public:
  /**
   * @brief Chart display types.
   */
  enum class Type : uint8_t {
    None = LV_CHART_TYPE_NONE,
    Line = LV_CHART_TYPE_LINE,
    Bar = LV_CHART_TYPE_BAR,
    Scatter = LV_CHART_TYPE_SCATTER,
  };

  /**
   * @brief Chart axis identifiers.
   */
  enum class Axis : uint8_t {
    PrimaryY = LV_CHART_AXIS_PRIMARY_Y,
    SecondaryY = LV_CHART_AXIS_SECONDARY_Y,
    PrimaryX = LV_CHART_AXIS_PRIMARY_X,
    SecondaryX = LV_CHART_AXIS_SECONDARY_X,
  };

  /**
   * @brief Chart update modes.
   */
  enum class UpdateMode : uint8_t {
    Shift = LV_CHART_UPDATE_MODE_SHIFT,
    Circular = LV_CHART_UPDATE_MODE_CIRCULAR,
  };

  Chart();
  explicit Chart(Object* parent, Ownership ownership = Ownership::Default);
  explicit Chart(Object& parent);
  explicit Chart(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Chart& set_type(Type type);
  Chart& set_point_count(uint32_t cnt);
  Chart& set_axis_range(Axis axis, int32_t min, int32_t max);
  Chart& set_div_line_count(uint32_t hdiv, uint32_t vdiv);
  Chart& set_update_mode(UpdateMode update_mode);

  Type get_type() const;
  uint32_t get_point_count() const;

  // Series management
  ChartSeries add_series(lv_color_t color, Axis axis);
  void remove_series(ChartSeries series);

  // Cursor management
  ChartCursor add_cursor(lv_color_t color, lv_dir_t dir);
  void remove_cursor(ChartCursor cursor);

  void refresh();
};

}  // namespace lvgl

#endif  // LV_USE_CHART
#endif  // LVGL_CPP_WIDGETS_CHART_H_
