#include "line.h"

#if LV_USE_LINE

namespace lvgl {

Line::Line() : Line(static_cast<Object*>(nullptr), Ownership::Managed) {}

Line::Line(Object* parent, Ownership ownership)
    : Widget(lv_line_create(parent ? parent->raw() : nullptr), ownership) {}

Line::Line(Object& parent) : Line(&parent) {}

Line::Line(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Line& Line::set_points(const PointPrecise points[], uint32_t point_num) {
  if (raw())
    lv_line_set_points(
        raw(), reinterpret_cast<const lv_point_precise_t*>(points), point_num);
  return *this;
}

Line& Line::set_points_mutable(PointPrecise points[], uint32_t point_num) {
  if (raw())
    lv_line_set_points_mutable(
        raw(), reinterpret_cast<lv_point_precise_t*>(points), point_num);
  return *this;
}

Line& Line::set_y_invert(bool en) {
  if (raw()) lv_line_set_y_invert(raw(), en);
  return *this;
}

const PointPrecise* Line::get_points() const {
  return raw()
             ? reinterpret_cast<const PointPrecise*>(lv_line_get_points(raw()))
             : nullptr;
}

uint32_t Line::get_point_count() const {
  return raw() ? lv_line_get_point_count(raw()) : 0;
}

bool Line::is_point_array_mutable() const {
  return raw() ? lv_line_is_point_array_mutable(raw()) : false;
}

PointPrecise* Line::get_points_mutable() {
  return raw() ? reinterpret_cast<PointPrecise*>(
                     lv_line_get_points_mutable(raw()))
               : nullptr;
}

bool Line::get_y_invert() const {
  return raw() ? lv_line_get_y_invert(raw()) : false;
}

}  // namespace lvgl

#endif  // LV_USE_LINE
