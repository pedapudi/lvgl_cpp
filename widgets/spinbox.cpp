#include "spinbox.h"

#if LV_USE_SPINBOX

namespace lvgl {

Spinbox::Spinbox() : Spinbox((Object*)nullptr) {}

Spinbox::Spinbox(Object* parent, Ownership ownership)
    : Textarea(lv_spinbox_create(parent ? parent->raw() : nullptr)) {}

Spinbox::Spinbox(lv_obj_t* obj, Ownership ownership) : Textarea(obj) {}

void Spinbox::set_value(int32_t v) {
  if (obj_) lv_spinbox_set_value(obj_, v);
}

void Spinbox::set_rollover(bool rollover) {
  if (obj_) lv_spinbox_set_rollover(obj_, rollover);
}

void Spinbox::set_digit_format(uint32_t digit_count, uint32_t sep_pos) {
  if (obj_) lv_spinbox_set_digit_format(obj_, digit_count, sep_pos);
}

void Spinbox::set_digit_count(uint32_t digit_count) {
  if (obj_) lv_spinbox_set_digit_count(obj_, digit_count);
}

void Spinbox::set_dec_point_pos(uint32_t dec_point_pos) {
  if (obj_) lv_spinbox_set_dec_point_pos(obj_, dec_point_pos);
}

void Spinbox::set_step(uint32_t step) {
  if (obj_) lv_spinbox_set_step(obj_, step);
}

void Spinbox::set_range(int32_t min_value, int32_t max_value) {
  if (obj_) lv_spinbox_set_range(obj_, min_value, max_value);
}

void Spinbox::set_min_value(int32_t min_value) {
  if (obj_) lv_spinbox_set_min_value(obj_, min_value);
}

void Spinbox::set_max_value(int32_t max_value) {
  if (obj_) lv_spinbox_set_max_value(obj_, max_value);
}

void Spinbox::set_cursor_pos(uint32_t pos) {
  if (obj_) lv_spinbox_set_cursor_pos(obj_, pos);
}

void Spinbox::set_digit_step_direction(lv_dir_t direction) {
  if (obj_) lv_spinbox_set_digit_step_direction(obj_, direction);
}

bool Spinbox::get_rollover() {
  return obj_ ? lv_spinbox_get_rollover(obj_) : false;
}

int32_t Spinbox::get_value() { return obj_ ? lv_spinbox_get_value(obj_) : 0; }

int32_t Spinbox::get_step() { return obj_ ? lv_spinbox_get_step(obj_) : 0; }

void Spinbox::step_next() {
  if (obj_) lv_spinbox_step_next(obj_);
}

void Spinbox::step_prev() {
  if (obj_) lv_spinbox_step_prev(obj_);
}

void Spinbox::increment() {
  if (obj_) lv_spinbox_increment(obj_);
}

void Spinbox::decrement() {
  if (obj_) lv_spinbox_decrement(obj_);
}

}  // namespace lvgl

#endif // LV_USE_SPINBOX
