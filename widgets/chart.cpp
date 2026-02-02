#include "chart.h"

#include "../misc/color.h"

#if LV_USE_CHART

namespace lvgl {

// --- ChartSeries ---

void ChartSeries::set_next_value(int32_t value) {
  if (chart_ && chart_->raw() && series_) {
    lv_chart_set_next_value(chart_->raw(), series_, value);
  }
}

void ChartSeries::set_next_value2(int32_t x_value, int32_t y_value) {
  if (chart_ && chart_->raw() && series_) {
    lv_chart_set_next_value2(chart_->raw(), series_, x_value, y_value);
  }
}

void ChartSeries::set_all_values(int32_t value) {
  if (chart_ && chart_->raw() && series_) {
    lv_chart_set_all_values(chart_->raw(), series_, value);
  }
}

void ChartSeries::set_color(Color color) {
  if (chart_ && chart_->raw() && series_) {
    lv_chart_set_series_color(chart_->raw(), series_, color);
  }
}

void ChartSeries::set_value_by_id(uint32_t id, int32_t value) {
  if (chart_ && chart_->raw() && series_) {
    lv_chart_set_series_value_by_id(chart_->raw(), series_, id, value);
  }
}

// --- Chart ---

Chart::Chart() : Chart(static_cast<Object*>(nullptr), Ownership::Managed) {}

Chart::Chart(Object* parent, Ownership ownership)
    : Widget(lv_chart_create(parent ? parent->raw() : nullptr), ownership) {}

Chart::Chart(Object& parent) : Chart(&parent) {}

Chart::Chart(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Chart& Chart::set_type(Type type) {
  if (raw()) lv_chart_set_type(raw(), static_cast<lv_chart_type_t>(type));
  return *this;
}

Chart& Chart::set_point_count(uint32_t cnt) {
  if (raw()) lv_chart_set_point_count(raw(), cnt);
  return *this;
}

Chart& Chart::set_axis_range(Axis axis, int32_t min, int32_t max) {
  if (raw())
    lv_chart_set_axis_range(raw(), static_cast<lv_chart_axis_t>(axis), min,
                            max);
  return *this;
}

Chart& Chart::set_div_line_count(uint32_t hdiv, uint32_t vdiv) {
  if (raw()) lv_chart_set_div_line_count(raw(), hdiv, vdiv);
  return *this;
}

Chart& Chart::set_update_mode(UpdateMode update_mode) {
  if (raw())
    lv_chart_set_update_mode(raw(),
                             static_cast<lv_chart_update_mode_t>(update_mode));
  return *this;
}

Chart::Type Chart::get_type() const {
  return raw() ? static_cast<Type>(lv_chart_get_type(raw())) : Type::None;
}

uint32_t Chart::get_point_count() const {
  return raw() ? lv_chart_get_point_count(raw()) : 0;
}

ChartSeries Chart::add_series(Color color, Axis axis) {
  if (raw()) {
    return ChartSeries(
        this,
        lv_chart_add_series(raw(), color, static_cast<lv_chart_axis_t>(axis)));
  }
  return ChartSeries(this, nullptr);
}

void Chart::remove_series(ChartSeries series) {
  if (raw() && series.raw()) {
    lv_chart_remove_series(raw(), series.raw());
  }
}

void Chart::refresh() {
  if (raw()) lv_chart_refresh(raw());
}

// --- ChartCursor ---

void ChartCursor::set_pos(Point pos) {
  if (chart_ && chart_->raw() && cursor_) {
    lv_point_t p = pos;
    lv_chart_set_cursor_pos(chart_->raw(), cursor_, &p);
  }
}

void ChartCursor::set_pos_x(int32_t x) {
  if (chart_ && chart_->raw() && cursor_) {
    lv_chart_set_cursor_pos_x(chart_->raw(), cursor_, x);
  }
}

void ChartCursor::set_pos_y(int32_t y) {
  if (chart_ && chart_->raw() && cursor_) {
    lv_chart_set_cursor_pos_y(chart_->raw(), cursor_, y);
  }
}

void ChartCursor::set_point(ChartSeries& series, uint32_t point_id) {
  if (chart_ && chart_->raw() && cursor_ && series.raw()) {
    lv_chart_set_cursor_point(chart_->raw(), cursor_, series.raw(), point_id);
  }
}

Point ChartCursor::get_point() const {
  if (chart_ && chart_->raw() && cursor_) {
    return Point(lv_chart_get_cursor_point(chart_->raw(), cursor_));
  }
  return Point(0, 0);
}

ChartCursor Chart::add_cursor(Color color, Dir dir) {
  if (raw()) {
    return ChartCursor(
        this, lv_chart_add_cursor(raw(), color, static_cast<lv_dir_t>(dir)));
  }
  return ChartCursor(this, nullptr);
}

void Chart::remove_cursor(ChartCursor cursor) {
  if (raw() && cursor.raw()) {
    lv_chart_remove_cursor(raw(), cursor.raw());
  }
}

}  // namespace lvgl

#endif  // LV_USE_CHART
