#pragma once

#include "driver/gpio.h"
#include "driver/spi_master.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_lcd_panel_vendor.h"

class Gc9a01 {
 public:
  struct Config {
    spi_host_device_t host;
    int cs_io_num;
    int dc_io_num;
    int sclk_io_num;
    int mosi_io_num;
    int bl_io_num;
    uint32_t pclk_hz;
    int h_res;
    int v_res;
  };

  explicit Gc9a01(const Config& config);
  ~Gc9a01();

  esp_err_t init();
  esp_lcd_panel_handle_t get_panel_handle() const { return panel_handle_; }
  esp_lcd_panel_io_handle_t get_io_handle() const { return io_handle_; }

 private:
  Config config_;
  esp_lcd_panel_io_handle_t io_handle_ = nullptr;
  esp_lcd_panel_handle_t panel_handle_ = nullptr;
};
