#include "slider.h"

#include "../core/observer.h"

#if LV_USE_SLIDER

namespace lvgl {

Slider::Slider() : Slider(static_cast<Object*>(nullptr), Ownership::Managed) {}

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

Slider& Slider::set_mode(Mode mode) {
  if (obj_) lv_slider_set_mode(obj_, static_cast<lv_slider_mode_t>(mode));
  return *this;
}

Slider& Slider::set_left_value(int32_t value, lv_anim_enable_t anim) {
  if (obj_) lv_slider_set_left_value(obj_, value, anim);
  return *this;
}

Slider& Slider::set_width(int32_t width) {
  Bar::set_width(width);
  return *this;
}

Slider& Slider::set_height(int32_t height) {
  Bar::set_height(height);
  return *this;
}

Slider& Slider::set_size(int32_t width, int32_t height) {
  Bar::set_size(width, height);
  return *this;
}

Slider& Slider::align(Align align, int32_t x_ofs, int32_t y_ofs) {
  Bar::align(align, x_ofs, y_ofs);
  return *this;
}

Slider& Slider::add_state(State state) {
  Bar::add_state(state);
  return *this;
}

Slider& Slider::remove_state(State state) {
  Bar::remove_state(state);
  return *this;
}

Slider& Slider::add_flag(lv_obj_flag_t flag) {
  Bar::add_flag(flag);
  return *this;
}

Slider& Slider::remove_flag(lv_obj_flag_t flag) {
  Bar::remove_flag(flag);
  return *this;
}

Slider& Slider::on_value_changed(Object::EventCallback cb) {
  add_event_cb(cb, LV_EVENT_VALUE_CHANGED);
  return *this;
}

int32_t Slider::get_left_value() const {
  return obj_ ? lv_slider_get_left_value(obj_) : 0;
}

bool Slider::is_dragged() const {
  return obj_ ? lv_slider_is_dragged(obj_) : false;
}

Observer Slider::bind_value(Subject& subject) {
  return Bar::bind_value(subject);
}

}  // namespace lvgl

#endif  // LV_USE_SLIDER
