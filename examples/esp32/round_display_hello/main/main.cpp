#include "driver/gpio.h"
#include "esp_log.h"
#include "hw/chsc6x.h"
#include "hw/gc9a01.h"
#include "sys/lvgl_port.h"
#include "ui/select_hello.h"

static const char* TAG = "main";

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "Starting Seeed Round Display Hello World");

  // 1. Hardware Initialization
  Gc9a01::Config display_cfg = {
      .host = SPI2_HOST,
      .cs_io_num = 1,
      .dc_io_num = 21,
      .sclk_io_num = 19,
      .mosi_io_num = 18,
      .bl_io_num = 16,
      .pclk_hz = 20 * 1000 * 1000,
      .h_res = 240,
      .v_res = 240,
  };
  Gc9a01 display_hw(display_cfg);
  display_hw.init();

  Chsc6x::Config touch_cfg = {
      .i2c_port = I2C_NUM_0,
      .sda_io_num = 22,
      .scl_io_num = 23,
      .int_io_num = 17,
      .clk_speed = 400000,
      .h_res = 240,
      .v_res = 240,
      .swap_xy = true,
      .mirror_x = true,
      .mirror_y = false,
  };
  Chsc6x chsc6x(touch_cfg);
  ESP_ERROR_CHECK(chsc6x.init());

  {
    // Initialize LVGL
    LvglPort::Config lvgl_config;
    lvgl_config.task_priority = 5;
    LvglPort lvgl_port(lvgl_config);
    lvgl_port.init(display_hw.get_panel_handle(), display_hw.get_io_handle());

    // Register touch driver
    lvgl_port.register_touch_driver(&chsc6x);

    // UI Initialization
    SelectHello select_hello;

    // Connect Backlight Control
    int bl_pin = display_cfg.bl_io_num;
    select_hello.set_on_backlight_changed([bl_pin](bool on) {
      gpio_num_t bl_gpio = (gpio_num_t)bl_pin;
      ESP_LOGI("main", "Backlight %s", on ? "ON" : "OFF");
      gpio_set_level(bl_gpio, on ? 1 : 0);
    });

    // Lock the mutex because we are modifying LVGL state
    if (lvgl_port.lock(-1)) {
      if (auto* display = lvgl_port.get_display()) {
        select_hello.show_menu(*display);
      }
      lvgl_port.unlock();
    }

    // Keep main task alive
    while (1) {
      vTaskDelay(pdMS_TO_TICKS(500));
    }
  }
}
