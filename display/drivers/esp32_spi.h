#pragma once

#include "../../lvgl_cpp.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"

namespace lvgl {

/**
 * @brief Generic ESP32 SPI Display Driver for Native Double Buffering.
 *
 * Supports full-frame SPIRAM double buffering compatible with
 * SPI panels (GC9A01, ST7789, etc.) using the esp_lcd component.
 */
class Esp32Spi {
 public:
  struct Config {
    int h_res;
    int v_res;
    esp_lcd_panel_handle_t panel_handle;
    esp_lcd_panel_io_handle_t io_handle;
    bool swap_bytes = true;  ///< Enable software (SIMD) byte swapping
    bool invert_colors =
        false;  ///< Invert colors during swap (active low panels)
  };

  /**
   * @brief Construct a new Esp32 Spi Driver
   *
   * @param config Driver configuration
   */
  explicit Esp32Spi(const Config& config);
  ~Esp32Spi();

  /**
   * @brief Get the underlying LVGL Display object
   *
   * @return lvgl::Display*
   */
  lvgl::Display* display() { return display_.get(); }

  /**
   * @brief Get the ESP-IDF Panel Handle.
   * Useful for low-level operations like power control or detailed
   * configuration.
   */
  esp_lcd_panel_handle_t get_panel_handle() const {
    return config_.panel_handle;
  }

  /**
   * @brief Get the ESP-IDF IO Handle.
   */
  esp_lcd_panel_io_handle_t get_io_handle() const { return config_.io_handle; }

 private:
  // SPI DMA completion callback
  static bool on_color_trans_done_trampoline(
      esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t* edata,
      void* user_ctx);

  // Buffer flush callback
  void flush_cb(lvgl::Display* disp, const lv_area_t* area, uint8_t* px_map);

  // Specialized flush implementations to avoid runtime branching
  void flush_no_processing(const lv_area_t* area, uint8_t* px_map);
  void flush_swap(const lv_area_t* area, uint8_t* px_map);
  void flush_swap_invert(const lv_area_t* area, uint8_t* px_map);

  using FlushImpl = void (Esp32Spi::*)(const lv_area_t*, uint8_t*);
  FlushImpl selected_flush_ = nullptr;

  Config config_;
  std::unique_ptr<lvgl::Display> display_;

  // Double Buffers in SPIRAM
  void* buf1_ = nullptr;
  void* buf2_ = nullptr;
  size_t buf_size_ = 0;
};

}  // namespace lvgl
