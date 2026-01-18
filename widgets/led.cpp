#include "led.h"

#if LV_USE_LED

namespace lvgl {

Led::Led() : Led(static_cast<Object*>(nullptr), Ownership::Managed) {}

Led::Led(Object* parent, Ownership ownership)
    : Widget(lv_led_create(parent ? parent->raw() : nullptr), ownership) {}

Led::Led(Object& parent) : Led(&parent) {}

Led::Led(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

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

uint8_t Led::get_brightness() { return obj_ ? lv_led_get_brightness(obj_) : 0; }

}  // namespace lvgl

#endif  // LV_USE_LED
