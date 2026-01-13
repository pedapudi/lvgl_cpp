#include "arc.h"

#include "../core/observer.h"

namespace lvgl {

Arc::Arc() : Arc((Object*)nullptr) {}

Arc::Arc(Object* parent)
    : Object(lv_arc_create(parent ? parent->raw() : nullptr)) {}

Arc::Arc(lv_obj_t* obj) : Object(obj) {}

void Arc::set_start_angle(lv_value_precise_t start) {
  if (obj_) lv_arc_set_start_angle(obj_, start);
}

void Arc::set_end_angle(lv_value_precise_t end) {
  if (obj_) lv_arc_set_end_angle(obj_, end);
}

void Arc::set_angles(lv_value_precise_t start, lv_value_precise_t end) {
  if (obj_) lv_arc_set_angles(obj_, start, end);
}

void Arc::set_bg_start_angle(lv_value_precise_t start) {
  if (obj_) lv_arc_set_bg_start_angle(obj_, start);
}

void Arc::set_bg_end_angle(lv_value_precise_t end) {
  if (obj_) lv_arc_set_bg_end_angle(obj_, end);
}

void Arc::set_bg_angles(lv_value_precise_t start, lv_value_precise_t end) {
  if (obj_) lv_arc_set_bg_angles(obj_, start, end);
}

void Arc::set_rotation(int32_t rotation) {
  if (obj_) lv_arc_set_rotation(obj_, rotation);
}

void Arc::set_mode(lv_arc_mode_t type) {
  if (obj_) lv_arc_set_mode(obj_, type);
}

void Arc::set_value(int32_t value) {
  if (obj_) lv_arc_set_value(obj_, value);
}

void Arc::set_range(int32_t min, int32_t max) {
  if (obj_) lv_arc_set_range(obj_, min, max);
}

void Arc::set_min_value(int32_t min) {
  if (obj_) lv_arc_set_min_value(obj_, min);
}

void Arc::set_max_value(int32_t max) {
  if (obj_) lv_arc_set_max_value(obj_, max);
}

void Arc::set_change_rate(uint32_t rate) {
  if (obj_) lv_arc_set_change_rate(obj_, rate);
}

void Arc::set_knob_offset(int32_t offset) {
  if (obj_) lv_arc_set_knob_offset(obj_, offset);
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
