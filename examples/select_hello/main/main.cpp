/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "core/lv_obj_tree.h"
#include "esp_log.h"
#include "hw/ssd1306_i2c.h"
#include "soc/gpio_num.h"
#include "sys/lvgl_port.h"
#include "ui/select_hello.h"

static const char* TAG = "main";

#define BUTTON_PIN1 GPIO_NUM_2
#define BUTTON_PIN2 GPIO_NUM_3

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

  gpio_config_t io_conf = {
      .pin_bit_mask = (1ULL << BUTTON_PIN1 | 1ULL << BUTTON_PIN2),  // Select GPIO 2
      .mode = GPIO_MODE_INPUT,                                      // Set as input
      .pull_up_en = GPIO_PULLUP_ENABLE,                             // Enable internal pull-up
      .pull_down_en = GPIO_PULLDOWN_DISABLE,                        // Disable pull-down
      .intr_type = GPIO_INTR_DISABLE                                // Disable interrupts
  };
  gpio_config(&io_conf);

  // 3. UI Initialization
  SelectHello select_hello;

  sys.lock(-1);
  if (auto* display = sys.get_display()) {
    display->set_rotation(LV_DISPLAY_ROTATION_180);
    select_hello.show_menu(*display);
  }
  sys.unlock();

  bool last_button1 = false;
  bool last_button2 = false;

  while (true) {
    // Read current state (inverted because pull-up, so low means pressed)
    bool button1_pressed = !gpio_get_level(BUTTON_PIN1);
    bool button2_pressed = !gpio_get_level(BUTTON_PIN2);

    // Simple edge detection
    bool btn1_clicked = button1_pressed && !last_button1;
    bool btn2_clicked = button2_pressed && !last_button2;

    last_button1 = button1_pressed;
    last_button2 = button2_pressed;

    if (btn1_clicked || btn2_clicked) {
      sys.lock(-1);
      // Map Button 1 to "Next" and Button 2 to "Enter"
      select_hello.handle_input(btn1_clicked, btn2_clicked);
      sys.unlock();
    }

    // Short delay for responsiveness
    vTaskDelay(pdMS_TO_TICKS(50));
  }
}
