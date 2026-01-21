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
  void read_cb(lv_indev_data_t* data);

  Config config_;
  lvgl::KeypadInput input_;
  uint32_t last_key_ = 0;
  lv_indev_state_t last_state_ = LV_INDEV_STATE_RELEASED;
};
