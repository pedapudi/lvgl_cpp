#include "line.h"

#if LV_USE_LINE

namespace lvgl {

Line::Line() : Line((Object*)nullptr) {}

Line::Line(Object* parent, Ownership ownership)
    : Object(lv_line_create(parent ? parent->raw() : nullptr), ownership) {}

Line::Line(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

void Line::set_points(const lv_point_precise_t points[], uint32_t point_num) {
  if (obj_) lv_line_set_points(obj_, points, point_num);
}

void Line::set_points_mutable(lv_point_precise_t points[], uint32_t point_num) {
  if (obj_) lv_line_set_points_mutable(obj_, points, point_num);
}

void Line::set_y_invert(bool en) {
  if (obj_) lv_line_set_y_invert(obj_, en);
}

const lv_point_precise_t* Line::get_points() const {
  return obj_ ? lv_line_get_points(obj_) : nullptr;
}

uint32_t Line::get_point_count() const {
  return obj_ ? lv_line_get_point_count(obj_) : 0;
}

bool Line::is_point_array_mutable() const {
  return obj_ ? lv_line_is_point_array_mutable(obj_) : false;
}

lv_point_precise_t* Line::get_points_mutable() {
  return obj_ ? lv_line_get_points_mutable(obj_) : nullptr;
}

bool Line::get_y_invert() const {
  return obj_ ? lv_line_get_y_invert(obj_) : false;
}

}  // namespace lvgl

#endif // LV_USE_LINE
