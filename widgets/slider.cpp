#include "slider.h"

#include "../core/observer.h"

#if LV_USE_SLIDER

namespace lvgl {

Slider::Slider() : Bar(lv_slider_create(nullptr), Ownership::Managed) {}

Slider::Slider(Object* parent, Ownership ownership)
    : Bar(lv_slider_create(parent ? parent->raw() : nullptr), ownership) {}

Slider::Slider(Object& parent) : Slider(&parent) {}

Slider::Slider(Object& parent, int32_t min, int32_t max) : Slider(parent) {
  set_range(min, max);
}

Slider::Slider(lv_obj_t* obj, Ownership ownership) : Bar(obj, ownership) {}

Slider& Slider::set_value(int32_t value, lv_anim_enable_t anim) {
  Bar::set_value(value, anim);
  return *this;
}

Slider& Slider::set_start_value(int32_t value, lv_anim_enable_t anim) {
  Bar::set_start_value(value, anim);
  return *this;
}

Slider& Slider::set_range(int32_t min, int32_t max) {
  Bar::set_range(min, max);
  return *this;
}

Slider& Slider::set_mode(lv_bar_mode_t mode) {
  Bar::set_mode(mode);
  return *this;
}

Slider& Slider::set_left_value(int32_t value, lv_anim_enable_t anim) {
  if (obj_) lv_slider_set_start_value(obj_, value, anim);
  return *this;
}

int32_t Slider::get_left_value() const {
  return obj_ ? lv_slider_get_left_value(obj_) : 0;
}

bool Slider::is_dragged() const {
  return obj_ ? lv_slider_is_dragged(obj_) : false;
}

Slider& Slider::set_width(int32_t width) {
  if (obj_) lv_obj_set_width(obj_, width);
  return *this;
}
Slider& Slider::set_height(int32_t height) {
  if (obj_) lv_obj_set_height(obj_, height);
  return *this;
}
Slider& Slider::set_size(int32_t width, int32_t height) {
  if (obj_) lv_obj_set_size(obj_, width, height);
  return *this;
}
Slider& Slider::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  if (obj_) lv_obj_align(obj_, static_cast<lv_align_t>(align), x_ofs, y_ofs);
  return *this;
}
Slider& Slider::add_state(lv_state_t state) {
  if (obj_) lv_obj_add_state(obj_, state);
  return *this;
}
Slider& Slider::remove_state(lv_state_t state) {
  if (obj_) lv_obj_remove_state(obj_, state);
  return *this;
}
Slider& Slider::add_flag(lv_obj_flag_t flag) {
  if (obj_) lv_obj_add_flag(obj_, flag);
  return *this;
}
Slider& Slider::remove_flag(lv_obj_flag_t flag) {
  if (obj_) lv_obj_remove_flag(obj_, flag);
  return *this;
}

Observer Slider::bind_value(Subject& subject) {
  return Observer(lv_slider_bind_value(raw(), subject.raw()));
}

}  // namespace lvgl

#endif  // LV_USE_SLIDER
