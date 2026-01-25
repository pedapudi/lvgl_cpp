#pragma once

#include "driver/gpio.h"
#include "lvgl_cpp/indev/keypad_input.h"

class GpioInput {
 public:
  struct Config {
    gpio_num_t next_pin = GPIO_NUM_2;
    gpio_num_t enter_pin = GPIO_NUM_3;
  };

  explicit GpioInput(const Config& config);

  lvgl::KeypadInput& get_input() { return input_; }

 private:
  void read_cb(lvgl::IndevData& data);

  Config config_;
  lvgl::KeypadInput input_;
  lvgl::Key last_key_ = lvgl::Key::Enter;
  lvgl::IndevState last_state_ = lvgl::IndevState::Released;
};
