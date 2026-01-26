#include "gc9a01.h"
// #include "driver/ledc.h"
#include "esp_intr_alloc.h"
#include "esp_lcd_gc9a01.h"
#include "esp_log.h"

static const char* TAG = "Gc9a01";

Gc9a01::Gc9a01(const Config& config) : config_(config) {}

Gc9a01::~Gc9a01() {
  if (panel_handle_) {
    esp_lcd_panel_del(panel_handle_);
  }
  if (io_handle_) {
    esp_lcd_panel_io_del(io_handle_);
  }
}

esp_err_t Gc9a01::init() {
  ESP_LOGI(TAG, "Initialize SPI bus");
  spi_bus_config_t buscfg = {
      .mosi_io_num = config_.mosi_io_num,
      .miso_io_num = -1,
      .sclk_io_num = config_.sclk_io_num,
      .quadwp_io_num = -1,
      .quadhd_io_num = -1,
      .data4_io_num = -1,
      .data5_io_num = -1,
      .data6_io_num = -1,
      .data7_io_num = -1,
      .data_io_default_level = 0,
      .max_transfer_sz = (int)(config_.h_res * 20 * sizeof(uint16_t)),
      .flags = 0,
      .isr_cpu_id = ESP_INTR_CPU_AFFINITY_AUTO,
      .intr_flags = 0,
  };
  ESP_ERROR_CHECK(spi_bus_initialize(config_.host, &buscfg, SPI_DMA_CH_AUTO));

  ESP_LOGI(TAG, "Install panel IO");
  esp_lcd_panel_io_spi_config_t io_config = {
      .cs_gpio_num = (gpio_num_t)config_.cs_io_num,
      .dc_gpio_num = (gpio_num_t)config_.dc_io_num,
      .spi_mode = 0,
      .pclk_hz = config_.pclk_hz,
      .trans_queue_depth = 10,
      .on_color_trans_done = NULL,
      .user_ctx = NULL,
      .lcd_cmd_bits = 8,
      .lcd_param_bits = 8,
      .cs_ena_pretrans = 0,
      .cs_ena_posttrans = 0,
      .flags =
          {
              .dc_high_on_cmd = 0,
              .dc_low_on_data = 0,
              .dc_low_on_param = 0,
              .octal_mode = 0,
              .quad_mode = 0,
              .sio_mode = 0,
              .lsb_first = 0,
              .cs_high_active = 0,
          },
  };
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_spi(
      (esp_lcd_spi_bus_handle_t)config_.host, &io_config, &io_handle_));

  /*
   * GC9A01 Panel Configuration
   * --------------------------
   * This section configures how the ESP32-C6 communicates with the display
   * driver.
   *
   * 1. rgb_ele_order: Set to BGR.
   *    Note: Depending on the specific panel revision, RGB or BGR might be
   * required. If Red and Blue appear swapped, toggle this value.
   *
   * 2. data_endian: Set to BIG.
   *    SPI displays typically expect Big Endian (MSB first) for 16-bit color
   * data. If this is wrong, you might see "shimmering" or "fringing" on
   * anti-aliased text.
   *
   * 3. bits_per_pixel: 16 (RGB565).
   */
  ESP_LOGI(TAG, "Install GC9A01 panel driver");
  esp_lcd_panel_dev_config_t panel_config = {
      .rgb_ele_order = LCD_RGB_ELEMENT_ORDER_BGR,
      .data_endian = LCD_RGB_DATA_ENDIAN_BIG,
      .bits_per_pixel = 16,
      .reset_gpio_num = GPIO_NUM_NC,
      .vendor_config = NULL,
      .flags = {.reset_active_high = 0},
  };
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_gc9a01(io_handle_, &panel_config, &panel_handle_));

  /*
   * Initialize and Power On the Display
   * -----------------------------------
   * We reset the chip, initialize it with the default vendor commands, and turn
   * it on.
   */
  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle_));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle_));

  /*
   * IMPORTANT: Color Inversion
   * --------------------------
   * This specific display hardware physically requires inverted data (0=Bright,
   * 1=Dark) to show colors correctly.
   *
   * While the driver supports hardware inversion via 'true' here, the user has
   * requested this be set to 'false' (disabled) globally.
   *
   * TO COMPENSATE: We perform the bitwise inversion manually in software inside
   * LvglPort's flush callback (lvgl_port.cpp).
   */
  ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle_, false));

  /*
   * Orientation and Mirroring
   * -------------------------
   * We swap XY and mirror to ensure the coordinate system (0,0) is at the
   * top-left in a way that matches the physical mounting of the round display.
   */
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle_, true));
  ESP_ERROR_CHECK(esp_lcd_panel_swap_xy(panel_handle_, true));
  ESP_ERROR_CHECK(esp_lcd_panel_mirror(panel_handle_, true, true));

  ESP_LOGI(TAG, "Initialize Backlight");
  gpio_config_t bk_gpio_config = {
      .pin_bit_mask = 1ULL << config_.bl_io_num,
      .mode = GPIO_MODE_OUTPUT,
      .pull_up_en = GPIO_PULLUP_DISABLE,
      .pull_down_en = GPIO_PULLDOWN_DISABLE,
      .intr_type = GPIO_INTR_DISABLE,
  };
  ESP_ERROR_CHECK(gpio_config(&bk_gpio_config));
  ESP_ERROR_CHECK(gpio_set_level((gpio_num_t)config_.bl_io_num, 1));

  return ESP_OK;
}
