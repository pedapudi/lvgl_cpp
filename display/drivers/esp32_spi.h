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
  /**
   * @brief Configuration for the ESP32 SPI Display Driver.
   */
  struct Config {
    int h_res;                            ///< Horizontal resolution
    int v_res;                            ///< Vertical resolution
    esp_lcd_panel_handle_t panel_handle;  ///< Pre-initialized LCD panel handle
    esp_lcd_panel_io_handle_t io_handle;  ///< Pre-initialized IO handle
    lvgl::Display::RenderMode render_mode =
        lvgl::Display::RenderMode::Full;  ///< Rendering strategy
    bool swap_bytes = true;      ///< Enable software (SIMD) byte swapping
    bool invert_colors = false;  ///< Invert colors during swap
  };

  /**
   * @brief Construct a new Generic ESP32 SPI Display Driver.
   *
   * ## User Guide: High Performance SPI Driver
   *
   * This driver provides a flexible high-performance interface for SPI-based
   * displays (e.g. ST7789, GC9A01, ILI9341). It leverages ESP32 DMA features to
   * provide asynchronous rendering and tearing prevention.
   *
   * ### 1. Full Double Buffering (Recommended for PSRAM)
   * This mode allocates two full framebuffers. It is best used on chips with
   * SPIRAM (like ESP32-S3 or ESP32-P4).
   *
   * **Advantages:**
   * - Zero Screen Tearing: Buffer switches are atomic relative to the transfer
   * end.
   * - Background Transfers: CPU is free to render Frame N+1 while DMA sends
   * Frame N.
   *
   * **Usage Example:**
   * ```cpp
   * lvgl::Esp32Spi::Config config = {
   *     .h_res = 240,
   *     .v_res = 240,
   *     .panel_handle = lcd_h,
   *     .io_handle = io_h,
   *     .render_mode = lvgl::Display::RenderMode::Full
   * };
   * auto display = std::make_unique<lvgl::Esp32Spi>(config);
   * ```
   *
   * ### 2. Partial Rendering (Memory Efficient)
   * Use this mode if your device is severely constrained (no PSRAM) or if you
   * want maximum rendering speed for complex widgets by using internal SRAM.
   *
   * **Performance Tuning:**
   * In this mode, two smaller buffers are allocated in Internal SRAM. Smaller
   * buffers are faster to render into but require more frequent context
   * switches between LVGL and the hardware driver.
   *
   * ### SIMD Byte Swapping Optimization
   * Most SPI panels expect Big-Endian data (MSB first). If your SPI controller
   * or LCD peripheral doesn't support hardware byte swapping, this driver
   * implements a hand-optimized **32-bit SWAR (SIMD-within-a-Register)**
   * software swap.
   *
   * This logic processes two pixels at a time and is loop-unrolled to maximize
   * throughput on Xtensa LX7 cores. It is significantly faster than the
   * standard LVGL software swap callback.
   *
   * ### Generality & Portability
   * This driver does not manage pins or initialize the SPI bus itself. Instead,
   * it accepts already-initialized `esp_lcd` handles. This allows you to use it
   * with any display controller supported by the ESP-IDF LCD component.
   *
   * @param config Driver configuration.
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
  // Buffer flush callback
  void flush_cb(const lv_area_t* area, uint8_t* px_map);

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
