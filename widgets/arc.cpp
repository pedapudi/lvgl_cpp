#include "arc.h"

#include "../core/observer.h"

#if LV_USE_ARC

namespace lvgl {

Arc::Arc() : Arc(static_cast<Object*>(nullptr), Ownership::Managed) {}

Arc::Arc(Object* parent, Ownership ownership)
    : Widget(lv_arc_create(parent ? parent->raw() : nullptr), ownership) {}

Arc::Arc(Object& parent) : Arc(&parent) {}

Arc::Arc(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

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

Arc& Arc::set_mode(Mode mode) {
  if (obj_) lv_arc_set_mode(obj_, static_cast<lv_arc_mode_t>(mode));
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

Arc::Mode Arc::get_mode() const {
  return obj_ ? static_cast<Mode>(lv_arc_get_mode(obj_)) : Mode::Normal;
}

int32_t Arc::get_rotation() const {
  return obj_ ? lv_arc_get_rotation(obj_) : 0;
}

int32_t Arc::get_knob_offset() const {
  return obj_ ? lv_arc_get_knob_offset(obj_) : 0;
}

Arc& Arc::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  add_event_cb(LV_EVENT_VALUE_CHANGED, cb);
  return *this;
}

Observer Arc::bind_value(Subject& subject) {
  return Observer(lv_arc_bind_value(raw(), subject.raw()));
}

}  // namespace lvgl

#endif  // LV_USE_ARC
