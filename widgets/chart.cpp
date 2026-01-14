#include "chart.h"

#if LV_USE_CHART

namespace lvgl {

Chart::Chart() : Chart((Object*)nullptr) {}

Chart::Chart(Object* parent)
    : Object(lv_chart_create(parent ? parent->raw() : nullptr)) {}

Chart::Chart(lv_obj_t* obj) : Object(obj) {}

void Chart::set_type(lv_chart_type_t type) {
  if (obj_) lv_chart_set_type(obj_, type);
}

void Chart::set_point_count(uint32_t cnt) {
  if (obj_) lv_chart_set_point_count(obj_, cnt);
}

void Chart::set_axis_range(lv_chart_axis_t axis, int32_t min, int32_t max) {
  if (obj_) lv_chart_set_axis_range(obj_, axis, min, max);
}

void Chart::set_div_line_count(uint32_t hdiv, uint32_t vdiv) {
  if (obj_) lv_chart_set_div_line_count(obj_, hdiv, vdiv);
}

void Chart::set_update_mode(lv_chart_update_mode_t update_mode) {
  if (obj_) lv_chart_set_update_mode(obj_, update_mode);
}

lv_chart_type_t Chart::get_type() {
  return obj_ ? lv_chart_get_type(obj_) : LV_CHART_TYPE_NONE;
}

uint32_t Chart::get_point_count() {
  return obj_ ? lv_chart_get_point_count(obj_) : 0;
}

lv_chart_series_t* Chart::add_series(lv_color_t color, lv_chart_axis_t axis) {
  return obj_ ? lv_chart_add_series(obj_, color, axis) : nullptr;
}

void Chart::remove_series(lv_chart_series_t* series) {
  if (obj_) lv_chart_remove_series(obj_, series);
}

void Chart::set_series_color(lv_chart_series_t* series, lv_color_t color) {
  if (obj_) lv_chart_set_series_color(obj_, series, color);
}

void Chart::set_next_value(lv_chart_series_t* series, int32_t value) {
  if (obj_) lv_chart_set_next_value(obj_, series, value);
}

void Chart::set_next_value2(lv_chart_series_t* series, int32_t x_value,
                            int32_t y_value) {
  if (obj_) lv_chart_set_next_value2(obj_, series, x_value, y_value);
}

void Chart::set_all_values(lv_chart_series_t* series, int32_t value) {
  if (obj_) lv_chart_set_all_values(obj_, series, value);
}

void Chart::refresh() {
  if (obj_) lv_chart_refresh(obj_);
}

}  // namespace lvgl

#endif // LV_USE_CHART
