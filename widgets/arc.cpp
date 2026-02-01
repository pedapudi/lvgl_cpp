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
  if (raw()) lv_arc_set_start_angle(raw(), start);
  return *this;
}

Arc& Arc::set_end_angle(lv_value_precise_t end) {
  if (raw()) lv_arc_set_end_angle(raw(), end);
  return *this;
}

Arc& Arc::set_angles(lv_value_precise_t start, lv_value_precise_t end) {
  if (raw()) lv_arc_set_angles(raw(), start, end);
  return *this;
}

Arc& Arc::set_bg_start_angle(lv_value_precise_t start) {
  if (raw()) lv_arc_set_bg_start_angle(raw(), start);
  return *this;
}

Arc& Arc::set_bg_end_angle(lv_value_precise_t end) {
  if (raw()) lv_arc_set_bg_end_angle(raw(), end);
  return *this;
}

Arc& Arc::set_bg_angles(lv_value_precise_t start, lv_value_precise_t end) {
  if (raw()) lv_arc_set_bg_angles(raw(), start, end);
  return *this;
}

Arc& Arc::set_rotation(int32_t rotation) {
  if (raw()) lv_arc_set_rotation(raw(), rotation);
  return *this;
}

Arc& Arc::set_mode(Mode mode) {
  if (raw()) lv_arc_set_mode(raw(), static_cast<lv_arc_mode_t>(mode));
  return *this;
}

Arc& Arc::set_value(int32_t value) {
  if (raw()) lv_arc_set_value(raw(), value);
  return *this;
}

Arc& Arc::set_range(int32_t min, int32_t max) {
  if (raw()) lv_arc_set_range(raw(), min, max);
  return *this;
}

Arc& Arc::set_min_value(int32_t min) {
  if (raw()) lv_arc_set_min_value(raw(), min);
  return *this;
}

Arc& Arc::set_max_value(int32_t max) {
  if (raw()) lv_arc_set_max_value(raw(), max);
  return *this;
}

Arc& Arc::set_change_rate(uint32_t rate) {
  if (raw()) lv_arc_set_change_rate(raw(), rate);
  return *this;
}

Arc& Arc::set_knob_offset(int32_t offset) {
  if (raw()) lv_arc_set_knob_offset(raw(), offset);
  return *this;
}

lv_value_precise_t Arc::get_angle_start() {
  return raw() ? lv_arc_get_angle_start(raw()) : 0;
}

lv_value_precise_t Arc::get_angle_end() {
  return raw() ? lv_arc_get_angle_end(raw()) : 0;
}

lv_value_precise_t Arc::get_bg_angle_start() {
  return raw() ? lv_arc_get_bg_angle_start(raw()) : 0;
}

lv_value_precise_t Arc::get_bg_angle_end() {
  return raw() ? lv_arc_get_bg_angle_end(raw()) : 0;
}

int32_t Arc::get_value() const { return raw() ? lv_arc_get_value(raw()) : 0; }

int32_t Arc::get_min_value() const {
  return raw() ? lv_arc_get_min_value(raw()) : 0;
}

int32_t Arc::get_max_value() const {
  return raw() ? lv_arc_get_max_value(raw()) : 100;
}

Arc::Mode Arc::get_mode() const {
  return raw() ? static_cast<Mode>(lv_arc_get_mode(raw())) : Mode::Normal;
}

int32_t Arc::get_rotation() const {
  return raw() ? lv_arc_get_rotation(raw()) : 0;
}

int32_t Arc::get_knob_offset() const {
  return raw() ? lv_arc_get_knob_offset(raw()) : 0;
}

Arc& Arc::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  add_event_cb(LV_EVENT_VALUE_CHANGED, cb);
  return *this;
}

Observer Arc::bind_value(Subject& subject) {
  return Observer(lv_arc_bind_value(raw(), subject.raw()), false);
}

void Arc::align_obj_to_angle(const Object& obj, int32_t r_offset) const {
  if (raw()) {
    lv_arc_align_obj_to_angle(raw(), obj.raw(), r_offset);
  }
}

void Arc::rotate_obj_to_angle(const Object& obj, int32_t r_offset) const {
  if (raw()) {
    lv_arc_rotate_obj_to_angle(raw(), obj.raw(), r_offset);
  }
}

}  // namespace lvgl

#endif  // LV_USE_ARC
