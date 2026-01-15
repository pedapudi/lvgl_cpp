#include "roller.h"

#include "../core/observer.h"

#if LV_USE_ROLLER

namespace lvgl {

Roller::Roller() : Roller((Object*)nullptr) {}

Roller::Roller(Object* parent, Ownership ownership)
    : Object(lv_roller_create(parent ? parent->raw() : nullptr), ownership) {}

Roller::Roller(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Roller& Roller::set_options(const char* options, lv_roller_mode_t mode) {
  if (obj_) lv_roller_set_options(obj_, options, mode);
  return *this;
}

Roller& Roller::set_selected(uint32_t sel_opt, lv_anim_enable_t anim) {
  if (obj_) lv_roller_set_selected(obj_, sel_opt, anim);
  return *this;
}

bool Roller::set_selected_str(const char* sel_opt, lv_anim_enable_t anim) {
  return obj_ ? lv_roller_set_selected_str(obj_, sel_opt, anim) : false;
}

Roller& Roller::set_visible_row_count(uint32_t row_cnt) {
  if (obj_) lv_roller_set_visible_row_count(obj_, row_cnt);
  return *this;
}

Roller& Roller::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Roller& Roller::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Roller& Roller::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Roller& Roller::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Roller& Roller::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Roller& Roller::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Roller& Roller::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Roller& Roller::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

uint32_t Roller::get_selected() {
  return obj_ ? lv_roller_get_selected(obj_) : 0;
}

void Roller::get_selected_str(char* buf, uint32_t buf_size) {
  if (obj_) lv_roller_get_selected_str(obj_, buf, buf_size);
}

const char* Roller::get_options() {
  return obj_ ? lv_roller_get_options(obj_) : nullptr;
}

uint32_t Roller::get_option_count() {
  return obj_ ? lv_roller_get_option_count(obj_) : 0;
}

lv_result_t Roller::get_option_str(uint32_t option, char* buf,
                                   uint32_t buf_size) {
  return obj_ ? lv_roller_get_option_str(obj_, option, buf, buf_size)
              : LV_RESULT_INVALID;
}

Observer Roller::bind_value(Subject& subject) {
  return Observer(lv_roller_bind_value(raw(), subject.raw()));
}

}  // namespace lvgl

#endif  // LV_USE_ROLLER
