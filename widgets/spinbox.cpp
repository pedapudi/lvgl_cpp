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
  if (raw()) lv_spinbox_set_value(raw(), v);
  return *this;
}

Spinbox& Spinbox::set_rollover(bool rollover) {
  if (raw()) lv_spinbox_set_rollover(raw(), rollover);
  return *this;
}

Spinbox& Spinbox::set_digit_format(uint32_t digit_count, uint32_t sep_pos) {
  if (raw()) lv_spinbox_set_digit_format(raw(), digit_count, sep_pos);
  return *this;
}

Spinbox& Spinbox::set_digit_count(uint32_t digit_count) {
  if (raw()) lv_spinbox_set_digit_count(raw(), digit_count);
  return *this;
}

Spinbox& Spinbox::set_dec_point_pos(uint32_t dec_point_pos) {
  if (raw()) lv_spinbox_set_dec_point_pos(raw(), dec_point_pos);
  return *this;
}

Spinbox& Spinbox::set_step(uint32_t step) {
  if (raw()) lv_spinbox_set_step(raw(), step);
  return *this;
}

Spinbox& Spinbox::set_range(int32_t min_value, int32_t max_value) {
  if (raw()) lv_spinbox_set_range(raw(), min_value, max_value);
  return *this;
}

Spinbox& Spinbox::set_min_value(int32_t min_value) {
  if (raw()) lv_spinbox_set_min_value(raw(), min_value);
  return *this;
}

Spinbox& Spinbox::set_max_value(int32_t max_value) {
  if (raw()) lv_spinbox_set_max_value(raw(), max_value);
  return *this;
}

Spinbox& Spinbox::set_cursor_pos(uint32_t pos) {
  if (raw()) lv_spinbox_set_cursor_pos(raw(), pos);
  return *this;
}

Spinbox& Spinbox::set_digit_step_direction(Dir direction) {
  if (raw())
    lv_spinbox_set_digit_step_direction(raw(),
                                        static_cast<lv_dir_t>(direction));
  return *this;
}

bool Spinbox::get_rollover() {
  return raw() ? lv_spinbox_get_rollover(raw()) : false;
}

int32_t Spinbox::get_value() { return raw() ? lv_spinbox_get_value(raw()) : 0; }

int32_t Spinbox::get_step() { return raw() ? lv_spinbox_get_step(raw()) : 0; }

Spinbox& Spinbox::step_next() {
  if (raw()) lv_spinbox_step_next(raw());
  return *this;
}

Spinbox& Spinbox::step_prev() {
  if (raw()) lv_spinbox_step_prev(raw());
  return *this;
}

Spinbox& Spinbox::increment() {
  if (raw()) lv_spinbox_increment(raw());
  return *this;
}

Spinbox& Spinbox::decrement() {
  if (raw()) lv_spinbox_decrement(raw());
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
Spinbox& Spinbox::add_flag(ObjFlag flag) {
  Object::add_flag(flag);
  return *this;
}

Spinbox& Spinbox::remove_flag(ObjFlag flag) {
  Object::remove_flag(flag);
  return *this;
}

Spinbox& Spinbox::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  add_event_cb(EventCode::ValueChanged, std::move(cb));
  return *this;
}
Observer Spinbox::bind_value(Subject& subject) {
  return Observer(lv_spinbox_bind_value(raw(), subject.raw()), false);
}

}  // namespace lvgl

#endif  // LV_USE_SPINBOX
