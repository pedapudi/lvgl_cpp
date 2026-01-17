#include "arc.h"

#include "../core/observer.h"

#if LV_USE_ARC

namespace lvgl {

Arc::Arc() : Object(lv_arc_create(nullptr), Ownership::Managed) {}

Arc::Arc(Object& parent, Ownership ownership)
    : Object(lv_arc_create(parent.raw()), ownership) {}

Arc::Arc(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Arc& Arc::set_start_angle(lv_value_precise_t start) {
  if (obj_) lv_arc_set_start_angle(obj_, start);
  return *this;
}

Arc& Arc::set_end_angle(lv_value_precise_t end) {
  if (obj_) lv_arc_set_end_angle(obj_, end);
  return *this;
}

Arc& Arc::set_angles(lv_value_precise_t start, lv_value_precise_t end) {
  if (obj_) lv_arc_set_angles(obj_, start, end);
  return *this;
}

Arc& Arc::set_bg_start_angle(lv_value_precise_t start) {
  if (obj_) lv_arc_set_bg_start_angle(obj_, start);
  return *this;
}

Arc& Arc::set_bg_end_angle(lv_value_precise_t end) {
  if (obj_) lv_arc_set_bg_end_angle(obj_, end);
  return *this;
}

Arc& Arc::set_bg_angles(lv_value_precise_t start, lv_value_precise_t end) {
  if (obj_) lv_arc_set_bg_angles(obj_, start, end);
  return *this;
}

Arc& Arc::set_rotation(int32_t rotation) {
  if (obj_) lv_arc_set_rotation(obj_, rotation);
  return *this;
}

Arc& Arc::set_mode(lv_arc_mode_t type) {
  if (obj_) lv_arc_set_mode(obj_, type);
  return *this;
}

Arc& Arc::set_value(int32_t value) {
  if (obj_) lv_arc_set_value(obj_, value);
  return *this;
}

Arc& Arc::set_range(int32_t min, int32_t max) {
  if (obj_) lv_arc_set_range(obj_, min, max);
  return *this;
}

Arc& Arc::set_min_value(int32_t min) {
  if (obj_) lv_arc_set_min_value(obj_, min);
  return *this;
}

Arc& Arc::set_max_value(int32_t max) {
  if (obj_) lv_arc_set_max_value(obj_, max);
  return *this;
}

Arc& Arc::set_change_rate(uint32_t rate) {
  if (obj_) lv_arc_set_change_rate(obj_, rate);
  return *this;
}

Arc& Arc::set_knob_offset(int32_t offset) {
  if (obj_) lv_arc_set_knob_offset(obj_, offset);
  return *this;
}

Arc& Arc::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Arc& Arc::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Arc& Arc::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Arc& Arc::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Arc& Arc::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Arc& Arc::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Arc& Arc::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Arc& Arc::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

lv_value_precise_t Arc::get_angle_start() {
  return obj_ ? lv_arc_get_angle_start(obj_) : 0;
}

lv_value_precise_t Arc::get_angle_end() {
  return obj_ ? lv_arc_get_angle_end(obj_) : 0;
}

lv_value_precise_t Arc::get_bg_angle_start() {
  return obj_ ? lv_arc_get_bg_angle_start(obj_) : 0;
}

lv_value_precise_t Arc::get_bg_angle_end() {
  return obj_ ? lv_arc_get_bg_angle_end(obj_) : 0;
}

int32_t Arc::get_value() const { return obj_ ? lv_arc_get_value(obj_) : 0; }

int32_t Arc::get_min_value() const {
  return obj_ ? lv_arc_get_min_value(obj_) : 0;
}

int32_t Arc::get_max_value() const {
  return obj_ ? lv_arc_get_max_value(obj_) : 100;
}

lv_arc_mode_t Arc::get_mode() const {
  return obj_ ? lv_arc_get_mode(obj_) : LV_ARC_MODE_NORMAL;
}

int32_t Arc::get_rotation() const {
  return obj_ ? lv_arc_get_rotation(obj_) : 0;
}

int32_t Arc::get_knob_offset() const {
  return obj_ ? lv_arc_get_knob_offset(obj_) : 0;
}

Observer Arc::bind_value(Subject& subject) {
  return Observer(lv_arc_bind_value(raw(), subject.raw()));
}

}  // namespace lvgl

#endif  // LV_USE_ARC
