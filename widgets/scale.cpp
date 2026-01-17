#include "scale.h"

#if LV_USE_SCALE

namespace lvgl {

Scale::Scale() : Scale((Object*)nullptr) {}

Scale::Scale(Object* parent, Ownership ownership)
    : Object(lv_scale_create(parent ? parent->raw() : nullptr), ownership) {}

Scale::Scale(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Scale& Scale::set_mode(lv_scale_mode_t mode) {
  if (obj_) lv_scale_set_mode(obj_, mode);
  return *this;
}

Scale& Scale::set_total_tick_count(uint32_t total_tick_count) {
  if (obj_) lv_scale_set_total_tick_count(obj_, total_tick_count);
  return *this;
}

Scale& Scale::set_major_tick_every(uint32_t major_tick_every) {
  if (obj_) lv_scale_set_major_tick_every(obj_, major_tick_every);
  return *this;
}

Scale& Scale::set_label_show(bool show_label) {
  if (obj_) lv_scale_set_label_show(obj_, show_label);
  return *this;
}

Scale& Scale::set_range(int32_t min, int32_t max) {
  if (obj_) lv_scale_set_range(obj_, min, max);
  return *this;
}

Scale& Scale::set_angle_range(uint32_t angle_range) {
  if (obj_) lv_scale_set_angle_range(obj_, angle_range);
  return *this;
}

Scale& Scale::set_rotation(int32_t rotation) {
  if (obj_) lv_scale_set_rotation(obj_, rotation);
  return *this;
}

Scale& Scale::set_line_needle_value(lv_obj_t* needle_line,
                                    int32_t needle_length, int32_t value) {
  if (obj_)
    lv_scale_set_line_needle_value(obj_, needle_line, needle_length, value);
  return *this;
}

Scale& Scale::set_image_needle_value(lv_obj_t* needle_img, int32_t value) {
  if (obj_) lv_scale_set_image_needle_value(obj_, needle_img, value);
  return *this;
}

Scale& Scale::set_text_src(const char* txt_src[]) {
  if (obj_) lv_scale_set_text_src(obj_, txt_src);
  return *this;
}

Scale& Scale::set_post_draw(bool en) {
  if (obj_) lv_scale_set_post_draw(obj_, en);
  return *this;
}

Scale& Scale::set_draw_ticks_on_top(bool en) {
  if (obj_) lv_scale_set_draw_ticks_on_top(obj_, en);
  return *this;
}

Scale& Scale::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Scale& Scale::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Scale& Scale::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Scale& Scale::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Scale& Scale::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Scale& Scale::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Scale& Scale::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Scale& Scale::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

lv_scale_mode_t Scale::get_mode() {
  return obj_ ? lv_scale_get_mode(obj_) : LV_SCALE_MODE_HORIZONTAL_BOTTOM;
}

int32_t Scale::get_total_tick_count() {
  return obj_ ? lv_scale_get_total_tick_count(obj_) : 0;
}

int32_t Scale::get_major_tick_every() {
  return obj_ ? lv_scale_get_major_tick_every(obj_) : 0;
}

bool Scale::get_label_show() {
  return obj_ ? lv_scale_get_label_show(obj_) : false;
}

uint32_t Scale::get_angle_range() {
  return obj_ ? lv_scale_get_angle_range(obj_) : 0;
}

int32_t Scale::get_range_min_value() {
  return obj_ ? lv_scale_get_range_min_value(obj_) : 0;
}

int32_t Scale::get_range_max_value() {
  return obj_ ? lv_scale_get_range_max_value(obj_) : 0;
}

}  // namespace lvgl

#endif  // LV_USE_SCALE
