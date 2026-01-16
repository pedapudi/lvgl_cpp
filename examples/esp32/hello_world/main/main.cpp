/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "esp_log.h"
#include "hw/ssd1306_i2c.h"
#include "sys/lvgl_port.h"
#include "ui/hello_world.h"

static const char *TAG = "main";

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "Starting Hello LVGL C++ Example");

  // 1. Hardware Initialization
  Ssd1306I2c::Config hw_config;
  hw_config.sda_io_num = GPIO_NUM_6;
  hw_config.scl_io_num = GPIO_NUM_7;
  // hw_config.i2c_addr = 0x3C; // Default
  // hw_config.v_res = 64; // Default

  Ssd1306I2c hw(hw_config);
  hw.init();

  // 2. System/LVGL Integration
  LvglPort::Config sys_config;
  // sys_config.h_res = 128; // Default
  // sys_config.v_res = 64; // Default

  LvglPort sys(sys_config);
  sys.init(hw.get_panel_handle(), hw.get_io_handle());

  // 3. UI Loading
  sys.lock(-1); // Lock before LVGL operations
  if (auto *display = sys.get_display()) {
    HelloUi::load(*display);
  }
  sys.unlock();

  ESP_LOGI(TAG, "Initialization complete. Entering idle loop.");
}
