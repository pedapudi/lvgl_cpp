#pragma once

#include "driver/i2c_master.h"
#include "esp_err.h"

class Chsc6x {
 public:
  struct Config {
    i2c_port_t i2c_port;
    int sda_io_num;
    int scl_io_num;
    int int_io_num;
    uint32_t clk_speed;
    uint16_t h_res;
    uint16_t v_res;
    bool swap_xy;
    bool mirror_x;
    bool mirror_y;
  };

  explicit Chsc6x(const Config& config);
  ~Chsc6x();

  esp_err_t init();
  esp_err_t read(uint16_t* x, uint16_t* y, bool* pressed);

 private:
  Config config_;
  i2c_master_bus_handle_t bus_handle_ = nullptr;
  i2c_master_dev_handle_t dev_handle_ = nullptr;
};
