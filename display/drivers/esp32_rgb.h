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
  struct Config {
    int h_res;
    int v_res;
    esp_lcd_panel_handle_t panel_handle;
  };

  explicit Esp32RgbDisplay(const Config& config);
  ~Esp32RgbDisplay();

  lvgl::Display* display() { return display_.get(); }

 private:
  // Trampoline for VSync callback (runs in ISR context)
  static bool on_vsync_trampoline(
      esp_lcd_panel_handle_t handle,
      const esp_lcd_rgb_panel_event_data_t* event_data, void* user_ctx);

  // Buffer flush callback (called by LVGL task)
  void flush_cb(lvgl::Display* disp, const lv_area_t* area, uint8_t* px_map);

  Config config_;
  std::unique_ptr<lvgl::Display> display_;

  // Double Buffers in SPIRAM
  void* buf1_ = nullptr;
  void* buf2_ = nullptr;
  size_t buf_size_ = 0;
};

}  // namespace lvgl

#endif  // __has_include("esp_lcd_panel_rgb.h")
