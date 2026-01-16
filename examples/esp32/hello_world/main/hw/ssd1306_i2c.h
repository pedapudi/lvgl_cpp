#pragma once

#include "driver/gpio.h"
#include "driver/i2c_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

class Ssd1306I2c {
public:
  struct Config {
    i2c_port_t i2c_port = I2C_NUM_0;
    gpio_num_t sda_io_num;
    gpio_num_t scl_io_num;
    gpio_num_t rst_io_num = GPIO_NUM_NC;
    uint8_t i2c_addr = 0x3C;
    uint32_t clk_speed_hz = 400 * 1000;
    uint16_t h_res = 128;
    uint16_t v_res = 64;
  };

  explicit Ssd1306I2c(const Config &config);
  ~Ssd1306I2c() = default;

  void init();

  esp_lcd_panel_handle_t get_panel_handle() const { return panel_handle_; }
  esp_lcd_panel_io_handle_t get_io_handle() const { return io_handle_; }

private:
  Config config_;
  i2c_master_bus_handle_t i2c_bus_ = nullptr;
  esp_lcd_panel_io_handle_t io_handle_ = nullptr;
  esp_lcd_panel_handle_t panel_handle_ = nullptr;
};
