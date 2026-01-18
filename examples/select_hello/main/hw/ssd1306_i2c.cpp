#include "ssd1306_i2c.h"
#include "esp_check.h"
#include "esp_lcd_panel_vendor.h"
#include "esp_log.h"

static const char *TAG = "Ssd1306I2c";

Ssd1306I2c::Ssd1306I2c(const Config &config) : config_(config) {}

void Ssd1306I2c::init() {
  ESP_LOGI(TAG, "Initialize I2C bus");
  i2c_master_bus_config_t bus_config = {};
  bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
  bus_config.glitch_ignore_cnt = 7;
  bus_config.i2c_port = config_.i2c_port;
  bus_config.sda_io_num = config_.sda_io_num;
  bus_config.scl_io_num = config_.scl_io_num;
  bus_config.flags.enable_internal_pullup = true;

  ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus_));

  ESP_LOGI(TAG, "Install panel IO");
  esp_lcd_panel_io_i2c_config_t io_config = {};
  io_config.dev_addr = config_.i2c_addr;
  io_config.scl_speed_hz = config_.clk_speed_hz;
  io_config.control_phase_bytes = 1; // According to SSD1306 datasheet
  io_config.lcd_cmd_bits = 8;
  io_config.lcd_param_bits = 8;
  io_config.dc_bit_offset = 6; // According to SSD1306 datasheet

  ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus_, &io_config, &io_handle_));

  ESP_LOGI(TAG, "Install SSD1306 panel driver");
  esp_lcd_panel_dev_config_t panel_config = {};
  panel_config.bits_per_pixel = 1;
  panel_config.reset_gpio_num = config_.rst_io_num;

  esp_lcd_panel_ssd1306_config_t ssd1306_config = {
      .height = static_cast<uint8_t>(config_.v_res),
  };
  panel_config.vendor_config = &ssd1306_config;

  ESP_ERROR_CHECK(
      esp_lcd_new_panel_ssd1306(io_handle_, &panel_config, &panel_handle_));

  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle_));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle_));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle_, true));
}
