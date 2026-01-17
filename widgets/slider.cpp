#include "slider.h"

#include "../core/observer.h"

#if LV_USE_SLIDER

namespace lvgl {

Slider::Slider() : Bar(lv_slider_create(nullptr), Ownership::Managed) {}

Slider::Slider(Object& parent, Ownership ownership)
    : Bar(lv_slider_create(parent.raw())) {}

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
  Object::set_width(width);
  return *this;
}
Slider& Slider::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Slider& Slider::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Slider& Slider::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Slider& Slider::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Slider& Slider::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Slider& Slider::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Slider& Slider::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

Observer Slider::bind_value(Subject& subject) {
  return Observer(lv_slider_bind_value(raw(), subject.raw()));
}

}  // namespace lvgl

#endif  // LV_USE_SLIDER
