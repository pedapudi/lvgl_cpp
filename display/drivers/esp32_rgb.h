#pragma once

#include "lvgl_cpp/display/display.h"
#include "lvgl_cpp/lvgl_cpp.h"

#if __has_include("esp_lcd_panel_rgb.h")
#include "esp_lcd_panel_rgb.h"

namespace lvgl {

/**
 * @brief Specialized Display wrapper for ESP32 S3 RGB Panels using
 * Direct/Double-Buffered Rendering with VSync synchronization.
 */
class Esp32RgbDisplay {
 public:
  /**
   * @brief Configuration for the ESP32 RGB Display Driver.
   */
  struct Config {
    int h_res;                            ///< Horizontal resolution
    int v_res;                            ///< Vertical resolution
    esp_lcd_panel_handle_t panel_handle;  ///< Pre-initialized LCD panel handle
    lvgl::Display::RenderMode render_mode =
        lvgl::Display::RenderMode::Full;  ///< Rendering strategy
    size_t partial_buf_size =
        0;  ///< Size of partial buffer in SRAM. If 0, uses 1/10th.
  };

  /**
   * @brief Construct a new ESP32 RGB Display Driver.
   *
   * ## User Guide: High Performance RGB Driver
   *
   * This driver is designed for ESP32-S3 systems with large RGB panels (e.g.
   * 480x480, 800x480). It provides two primary modes of operation, selectable
   * via @ref Config::render_mode.
   *
   * ### 1. Full Double Buffering (Default)
   * In this mode, two full-sized framebuffers are allocated in SPIRAM. This is
   * the "Zero-Copy" path. LVGL renders directly into the buffer that the LCD
   * hardware will eventually scan out.
   *
   * **Advantages:**
   * - Perfectly smooth, tear-free animations.
   * - Zero CPU copy overhead between LVGL and the hardware.
   *
   * **Disadvantages:**
   * - High Memory Usage: Requires `2 * h_res * v_res * sizeof(color_t)` in
   * SPIRAM.
   * - Rendering Penalty: Complex alpha-blended UI elements may be slow to
   * render directly in PSRAM.
   *
   * **Usage Example:**
   * ```cpp
   * lvgl::Esp32RgbDisplay::Config config = {
   *     .h_res = 480,
   *     .v_res = 480,
   *     .panel_handle = my_lcd_handle,
   *     .render_mode = lvgl::Display::RenderMode::Full
   * };
   * auto display = std::make_unique<lvgl::Esp32RgbDisplay>(config);
   * ```
   *
   * ### 2. Partial Rendering with SRAM-to-PSRAM Copy
   * In this mode, a smaller "scratchpad" buffer is allocated in fast internal
   * SRAM, while two full framebuffers remain in PSRAM for hardware scanning.
   * LVGL renders into the fast SRAM buffer, and the results are copied back to
   * PSRAM during the flush callback.
   *
   * **Advantages:**
   * - Dramatically Faster Rendering: Alpha blending, shadows, and gradients are
   * computed in low-latency internal SRAM instead of high-latency external
   * PSRAM.
   * - Often yields higher overall FPS than "Full" mode for heavy UIs.
   *
   * **Usage Example:**
   * ```cpp
   * lvgl::Esp32RgbDisplay::Config config = {
   *     .h_res = 480,
   *     .v_res = 480,
   *     .panel_handle = my_lcd_handle,
   *     .render_mode = lvgl::Display::RenderMode::Partial,
   *     .partial_buf_size = 480 * 40 * 2 // Render 40 lines at a time in SRAM
   * };
   * auto display = std::make_unique<lvgl::Esp32RgbDisplay>(config);
   * ```
   *
   * ### VSync Synchronization
   * Regardless of the render mode, this driver ensures your application is
   * locked to the display's refresh rate. When a frame is submitted via @ref
   * flush_cb, the driver waits for the hardware VSync interrupt to fire before
   * notifying LVGL that the previous buffer is ready to be reused. This
   * effectively eliminates "screen tearing."
   *
   * @param config Driver configuration.
   */
  explicit Esp32RgbDisplay(const Config& config);
  ~Esp32RgbDisplay();

  lvgl::Display* display() { return display_.get(); }

 private:
  // Trampoline for VSync callback (runs in ISR context)
  static bool on_vsync_trampoline(
      esp_lcd_panel_handle_t handle,
      const esp_lcd_rgb_panel_event_data_t* event_data, void* user_ctx);

  // Buffer flush callback (called by LVGL task)
  void flush_cb(const lv_area_t* area, uint8_t* px_map);

  Config config_;
  std::unique_ptr<lvgl::Display> display_;

  // Double Buffers in SPIRAM
  void* buf1_ = nullptr;
  void* buf2_ = nullptr;
  void* sram_buf_ = nullptr;
  size_t buf_size_ = 0;
};

}  // namespace lvgl

#endif  // __has_include("esp_lcd_panel_rgb.h")
