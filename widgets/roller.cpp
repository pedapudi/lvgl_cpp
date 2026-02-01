#include "roller.h"

#include "../core/observer.h"

#if LV_USE_ROLLER

namespace lvgl {

Roller::Roller() : Roller(static_cast<Object*>(nullptr), Ownership::Managed) {}

Roller::Roller(Object* parent, Ownership ownership)
    : Widget(lv_roller_create(parent ? parent->raw() : nullptr), ownership) {}

Roller::Roller(Object& parent) : Roller(&parent) {}

Roller::Roller(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Roller& Roller::set_options(const char* options, Mode mode) {
  if (raw())
    lv_roller_set_options(raw(), options, static_cast<lv_roller_mode_t>(mode));
  return *this;
}

Roller& Roller::set_selected(uint32_t sel_opt, AnimEnable anim) {
  if (raw())
    lv_roller_set_selected(raw(), sel_opt, static_cast<lv_anim_enable_t>(anim));
  return *this;
}

bool Roller::set_selected_str(const char* sel_opt, AnimEnable anim) {
  return raw() ? lv_roller_set_selected_str(raw(), sel_opt,
                                            static_cast<lv_anim_enable_t>(anim))
               : false;
}

Roller& Roller::set_visible_row_count(uint32_t row_cnt) {
  if (raw()) lv_roller_set_visible_row_count(raw(), row_cnt);
  return *this;
}

Roller& Roller::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  return add_event_cb(LV_EVENT_VALUE_CHANGED, cb);
}

uint32_t Roller::get_selected() {
  return raw() ? lv_roller_get_selected(raw()) : 0;
}

void Roller::get_selected_str(char* buf, uint32_t buf_size) {
  if (raw()) lv_roller_get_selected_str(raw(), buf, buf_size);
}

const char* Roller::get_options() {
  return raw() ? lv_roller_get_options(raw()) : nullptr;
}

uint32_t Roller::get_option_count() {
  return raw() ? lv_roller_get_option_count(raw()) : 0;
}

lv_result_t Roller::get_option_str(uint32_t option, char* buf,
                                   uint32_t buf_size) {
  return raw() ? lv_roller_get_option_str(raw(), option, buf, buf_size)
               : LV_RESULT_INVALID;
}

Observer Roller::bind_value(Subject& subject) {
  return Observer(lv_roller_bind_value(raw(), subject.raw()), false);
}

}  // namespace lvgl

#endif  // LV_USE_ROLLER
