#include "led.h"

namespace lvgl {

Led::Led() : Led((Object*)nullptr) {}

Led::Led(Object* parent)
    : Object(lv_led_create(parent ? parent->raw() : nullptr)) {}

Led::Led(lv_obj_t* obj) : Object(obj) {}

void Led::set_color(lv_color_t color) {
  if (obj_) lv_led_set_color(obj_, color);
}

void Led::set_brightness(uint8_t bright) {
  if (obj_) lv_led_set_brightness(obj_, bright);
}

void Led::on() {
  if (obj_) lv_led_on(obj_);
}

void Led::off() {
  if (obj_) lv_led_off(obj_);
}

void Led::toggle() {
  if (obj_) lv_led_toggle(obj_);
}

uint8_t Led::get_brightness() { return obj_ ? lv_led_get_brightness(obj_) : 0; }

}  // namespace lvgl
