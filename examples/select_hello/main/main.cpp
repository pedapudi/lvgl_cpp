/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include "core/lv_obj_tree.h"
#include "esp_log.h"
#include "hw/ssd1306_i2c.h"
#include "sys/gpio_input.h"
#include "sys/lvgl_port.h"
#include "ui/select_hello.h"

static const char* TAG = "main";

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

  // 3. Input Initialization
  GpioInput::Config input_config;
  input_config.next_pin = GPIO_NUM_2;
  input_config.enter_pin = GPIO_NUM_3;
  GpioInput gpio_input(input_config);

  // 4. UI Initialization
  SelectHello select_hello;

  sys.lock(-1);
  if (auto* display = sys.get_display()) {
    display->set_rotation(lvgl::Display::Rotation::ROT_180);
    select_hello.show_menu(*display);

    /**
     * Bind the Input Device to the UI Group.
     *
     * In LVGL, navigation (like moving focus between buttons or scrolling a list)
     * is managed through "Groups". An Input Device (in our case, the physical buttons
     * wrapped by GpioInput) must be assigned to a Group so it knows which UI
     * elements it should interact with.
     *
     * How it works:
     * 1. GpioInput (Hardware Layer): Monitors the physical GPIO pins.
     * 2. KeypadInput (LVGL Layer): When a button is pressed, GpioInput tells
     *    this logical device that a "Key" (like 'Next' or 'Enter') was pressed.
     * 3. Group (UI Layer): Because we call `set_group()` here, the logical
     *    Input Device sends those keys to the Group managed by SelectHello.
     * 4. Interaction: The Group then handles the "Focus" — for example,
     *    telling the Roller to scroll to the next item when 'Next' is received.
     */
    gpio_input.get_input().set_group(select_hello.get_group());
  }
  sys.unlock();

  while (true) {
    // Keep the task alive (Input and UI are stack allocated)
    vTaskDelay(pdMS_TO_TICKS(1000));
  }
}
