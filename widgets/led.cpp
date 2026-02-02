#include "led.h"

#include "../misc/color.h"

#if LV_USE_LED

namespace lvgl {

Led::Led() : Led(static_cast<Object*>(nullptr), Ownership::Managed) {}

Led::Led(Object* parent, Ownership ownership)
    : Widget(lv_led_create(parent ? parent->raw() : nullptr), ownership) {}

Led::Led(Object& parent) : Led(&parent) {}

Led::Led(lv_obj_t* obj, Ownership ownership) : Widget(obj, ownership) {}

Led& Led::set_color(Color color) {
  if (raw()) lv_led_set_color(raw(), color);
  return *this;
}

Led& Led::set_brightness(uint8_t bright) {
  if (raw()) lv_led_set_brightness(raw(), bright);
  return *this;
}

Led& Led::on() {
  if (raw()) lv_led_on(raw());
  return *this;
}

Led& Led::off() {
  if (raw()) lv_led_off(raw());
  return *this;
}

Led& Led::toggle() {
  if (raw()) lv_led_toggle(raw());
  return *this;
}

uint8_t Led::get_brightness() { return raw() ? lv_led_get_brightness(raw()) : 0; }

}  // namespace lvgl

#endif  // LV_USE_LED
