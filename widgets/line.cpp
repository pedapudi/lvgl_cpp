#include "line.h"

#if LV_USE_LINE

namespace lvgl {

Line::Line() : Object(lv_line_create(nullptr), Ownership::Managed) {}

Line::Line(Object& parent, Ownership ownership)
    : Object(lv_line_create(parent.raw()), ownership) {}

Line::Line(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Line& Line::set_points(const lv_point_precise_t points[], uint32_t point_num) {
  if (obj_) lv_line_set_points(obj_, points, point_num);
  return *this;
}

Line& Line::set_points_mutable(lv_point_precise_t points[],
                               uint32_t point_num) {
  if (obj_) lv_line_set_points_mutable(obj_, points, point_num);
  return *this;
}

Line& Line::set_y_invert(bool en) {
  if (obj_) lv_line_set_y_invert(obj_, en);
  return *this;
}

Line& Line::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Line& Line::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Line& Line::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Line& Line::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Line& Line::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Line& Line::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Line& Line::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Line& Line::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
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

#endif  // LV_USE_LINE
