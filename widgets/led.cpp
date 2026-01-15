#include "led.h"

#if LV_USE_LED

namespace lvgl {

Led::Led() : Led((Object*)nullptr) {}

Led::Led(Object* parent, Ownership ownership)
    : Object(lv_led_create(parent ? parent->raw() : nullptr), ownership) {}

Led::Led(lv_obj_t* obj, Ownership ownership) : Object(obj, ownership) {}

Led& Led::set_color(lv_color_t color) {
  if (obj_) lv_led_set_color(obj_, color);
  return *this;
}

Led& Led::set_brightness(uint8_t bright) {
  if (obj_) lv_led_set_brightness(obj_, bright);
  return *this;
}

Led& Led::on() {
  if (obj_) lv_led_on(obj_);
  return *this;
}

Led& Led::off() {
  if (obj_) lv_led_off(obj_);
  return *this;
}

Led& Led::toggle() {
  if (obj_) lv_led_toggle(obj_);
  return *this;
}

Led& Led::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Led& Led::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Led& Led::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Led& Led::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Led& Led::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Led& Led::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Led& Led::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Led& Led::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
}

uint8_t Led::get_brightness() { return obj_ ? lv_led_get_brightness(obj_) : 0; }

}  // namespace lvgl

#endif  // LV_USE_LED
