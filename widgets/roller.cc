#include "roller.h"

#include "../core/observer.h"

namespace lvgl {

Roller::Roller() : Roller((Object*)nullptr) {}

Roller::Roller(Object* parent)
    : Object(lv_roller_create(parent ? parent->raw() : nullptr)) {}

Roller::Roller(lv_obj_t* obj) : Object(obj) {}

void Roller::set_options(const char* options, lv_roller_mode_t mode) {
  if (obj_) lv_roller_set_options(obj_, options, mode);
}

void Roller::set_selected(uint32_t sel_opt, lv_anim_enable_t anim) {
  if (obj_) lv_roller_set_selected(obj_, sel_opt, anim);
}

bool Roller::set_selected_str(const char* sel_opt, lv_anim_enable_t anim) {
  return obj_ ? lv_roller_set_selected_str(obj_, sel_opt, anim) : false;
}

void Roller::set_visible_row_count(uint32_t row_cnt) {
  if (obj_) lv_roller_set_visible_row_count(obj_, row_cnt);
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
