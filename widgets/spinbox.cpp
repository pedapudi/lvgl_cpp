#include "spinbox.h"

#include "lvgl.h"

#if LV_USE_SPINBOX

namespace lvgl {

Spinbox::Spinbox()
    : Spinbox(static_cast<Object*>(nullptr), Ownership::Managed) {}

Spinbox::Spinbox(Object* parent, Ownership ownership)
    : Textarea(lv_spinbox_create(parent ? parent->raw() : nullptr), ownership) {
}

Spinbox::Spinbox(Object& parent) : Spinbox(&parent) {}

Spinbox::Spinbox(lv_obj_t* obj, Ownership ownership)
    : Textarea(obj, ownership) {}

Spinbox& Spinbox::set_value(int32_t v) {
  if (obj_) lv_spinbox_set_value(obj_, v);
  return *this;
}

Spinbox& Spinbox::set_rollover(bool rollover) {
  if (obj_) lv_spinbox_set_rollover(obj_, rollover);
  return *this;
}

Spinbox& Spinbox::set_digit_format(uint32_t digit_count, uint32_t sep_pos) {
  if (obj_) lv_spinbox_set_digit_format(obj_, digit_count, sep_pos);
  return *this;
}

Spinbox& Spinbox::set_digit_count(uint32_t digit_count) {
  if (obj_) lv_spinbox_set_digit_count(obj_, digit_count);
  return *this;
}

Spinbox& Spinbox::set_dec_point_pos(uint32_t dec_point_pos) {
  if (obj_) lv_spinbox_set_dec_point_pos(obj_, dec_point_pos);
  return *this;
}

Spinbox& Spinbox::set_step(uint32_t step) {
  if (obj_) lv_spinbox_set_step(obj_, step);
  return *this;
}

Spinbox& Spinbox::set_range(int32_t min_value, int32_t max_value) {
  if (obj_) lv_spinbox_set_range(obj_, min_value, max_value);
  return *this;
}

Spinbox& Spinbox::set_min_value(int32_t min_value) {
  if (obj_) lv_spinbox_set_min_value(obj_, min_value);
  return *this;
}

Spinbox& Spinbox::set_max_value(int32_t max_value) {
  if (obj_) lv_spinbox_set_max_value(obj_, max_value);
  return *this;
}

Spinbox& Spinbox::set_cursor_pos(uint32_t pos) {
  if (obj_) lv_spinbox_set_cursor_pos(obj_, pos);
  return *this;
}

Spinbox& Spinbox::set_digit_step_direction(Dir direction) {
  if (obj_)
    lv_spinbox_set_digit_step_direction(obj_, static_cast<lv_dir_t>(direction));
  return *this;
}

Spinbox& Spinbox::set_digit_step_direction(lv_dir_t direction) {
  if (obj_) lv_spinbox_set_digit_step_direction(obj_, direction);
  return *this;
}

bool Spinbox::get_rollover() {
  return obj_ ? lv_spinbox_get_rollover(obj_) : false;
}

int32_t Spinbox::get_value() { return obj_ ? lv_spinbox_get_value(obj_) : 0; }

int32_t Spinbox::get_step() { return obj_ ? lv_spinbox_get_step(obj_) : 0; }

Spinbox& Spinbox::step_next() {
  if (obj_) lv_spinbox_step_next(obj_);
  return *this;
}

Spinbox& Spinbox::step_prev() {
  if (obj_) lv_spinbox_step_prev(obj_);
  return *this;
}

Spinbox& Spinbox::increment() {
  if (obj_) lv_spinbox_increment(obj_);
  return *this;
}

Spinbox& Spinbox::decrement() {
  if (obj_) lv_spinbox_decrement(obj_);
  return *this;
}

Spinbox& Spinbox::set_width(int32_t width) {
  Widget::set_width(width);
  return *this;
}
Spinbox& Spinbox::set_height(int32_t height) {
  Widget::set_height(height);
  return *this;
}
Spinbox& Spinbox::set_size(int32_t width, int32_t height) {
  Widget::set_size(width, height);
  return *this;
}
Spinbox& Spinbox::align(lvgl::Align align, int32_t x_ofs, int32_t y_ofs) {
  Widget::align(align, x_ofs, y_ofs);
  return *this;
}
Spinbox& Spinbox::add_state(State state) {
  this->state().add(state);
  return *this;
}

Spinbox& Spinbox::remove_state(State state) {
  this->state().remove(state);
  return *this;
}
Spinbox& Spinbox::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}

Spinbox& Spinbox::add_flag(ObjFlag flag) {
  Object::add_flag(flag);
  return *this;
}

Spinbox& Spinbox::remove_flag(lv_obj_flag_t flag) {
  if (obj_) lv_obj_remove_flag(obj_, flag);
  return *this;
}

Spinbox& Spinbox::remove_flag(ObjFlag flag) {
  Object::remove_flag(flag);
  return *this;
}

Spinbox& Spinbox::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  add_event_cb(LV_EVENT_VALUE_CHANGED, cb);
  return *this;
}
Observer Spinbox::bind_value(Subject& subject) {
  return Observer(lv_spinbox_bind_value(raw(), subject.raw()), false);
}

}  // namespace lvgl

#endif  // LV_USE_SPINBOX
