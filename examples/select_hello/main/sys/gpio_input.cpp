#include "gpio_input.h"

#include "lvgl_cpp/misc/enums.h"

GpioInput::GpioInput(const Config& config) : config_(config) {
  // Initialize GPIOs
  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << config_.next_pin) | (1ULL << config_.enter_pin),
      .mode = GPIO_MODE_INPUT,
      .pull_up_en = GPIO_PULLUP_ENABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE};
  gpio_config(&io_conf);

  // Create Keypad Input
  input_ = lvgl::KeypadInput::create();

  // Set the idiomatic read callback
  input_.set_read_cb([this](lvgl::IndevData& data) { this->read_cb(data); });
}

void GpioInput::read_cb(lvgl::IndevData& data) {
  // Read the physical state of the GPIO pins (active low buttons).
  bool next_pressed = !gpio_get_level(config_.next_pin);
  bool enter_pressed = !gpio_get_level(config_.enter_pin);

  // Logic: Map the physical buttons to LVGL virtual keys.
  if (enter_pressed) {
    data.set_state(lvgl::IndevState::Pressed).set_key(lvgl::Key::Enter);
    last_key_ = lvgl::Key::Enter;
    last_state_ = lvgl::IndevState::Pressed;
  } else if (next_pressed) {
    // Because our application uses a Roller which handles UP/DOWN natively for
    // scrolling, we map our "Next" button to LV_KEY_DOWN.
    data.set_state(lvgl::IndevState::Pressed).set_key(lvgl::Key::Down);
    last_key_ = lvgl::Key::Down;
    last_state_ = lvgl::IndevState::Pressed;
  } else {
    data.set_state(lvgl::IndevState::Released).set_key(last_key_);
    last_state_ = lvgl::IndevState::Released;
  }
}
