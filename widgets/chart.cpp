#include "chart.h"

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

void ChartSeries::set_color(lv_color_t color) {
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
  if (obj_) lv_chart_set_type(obj_, static_cast<lv_chart_type_t>(type));
  return *this;
}

Chart& Chart::set_point_count(uint32_t cnt) {
  if (obj_) lv_chart_set_point_count(obj_, cnt);
  return *this;
}

Chart& Chart::set_axis_range(Axis axis, int32_t min, int32_t max) {
  if (obj_)
    lv_chart_set_axis_range(obj_, static_cast<lv_chart_axis_t>(axis), min, max);
  return *this;
}

Chart& Chart::set_div_line_count(uint32_t hdiv, uint32_t vdiv) {
  if (obj_) lv_chart_set_div_line_count(obj_, hdiv, vdiv);
  return *this;
}

Chart& Chart::set_update_mode(UpdateMode update_mode) {
  if (obj_)
    lv_chart_set_update_mode(obj_,
                             static_cast<lv_chart_update_mode_t>(update_mode));
  return *this;
}

Chart::Type Chart::get_type() const {
  return obj_ ? static_cast<Type>(lv_chart_get_type(obj_)) : Type::None;
}

uint32_t Chart::get_point_count() const {
  return obj_ ? lv_chart_get_point_count(obj_) : 0;
}

ChartSeries Chart::add_series(lv_color_t color, Axis axis) {
  if (obj_) {
    return ChartSeries(
        this,
        lv_chart_add_series(obj_, color, static_cast<lv_chart_axis_t>(axis)));
  }
  return ChartSeries(this, nullptr);
}

void Chart::remove_series(ChartSeries series) {
  if (obj_ && series.raw()) {
    lv_chart_remove_series(obj_, series.raw());
  }
}

void Chart::refresh() {
  if (obj_) lv_chart_refresh(obj_);
}

}  // namespace lvgl

#endif  // LV_USE_CHART
