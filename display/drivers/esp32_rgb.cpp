#include "lvgl_cpp/display/drivers/esp32_rgb.h"

#if __has_include("esp_lcd_panel_rgb.h")
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"

static const char* TAG = "Esp32RgbDisplay";

namespace lvgl {

Esp32RgbDisplay::Esp32RgbDisplay(const Config& config) : config_(config) {
  // 1. Allocate Double Buffers in SPIRAM (DMA capable)
  // Each buffer is 2 bytes per pixel (RGB565)
  size_t frame_bytes = config.h_res * config.v_res * sizeof(uint16_t);
  buf_size_ = frame_bytes;

  // Align to cache line size (usually 64 bytes for S3)
  buf1_ = heap_caps_aligned_alloc(64, buf_size_,
                                  MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
  buf2_ = heap_caps_aligned_alloc(64, buf_size_,
                                  MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);

  if (!buf1_ || !buf2_) {
    ESP_LOGE(TAG, "Failed to allocate double buffers in SPIRAM");
    return;
  }

  // 2. Create LVGL Display
  display_ = std::make_unique<lvgl::Display>(config.h_res, config.v_res);
  display_->set_color_format(lvgl::ColorFormat::RGB565);

  // 3. Set Buffers (Full Mode = Ping Pong)
  // LVGL will rotate between these two buffers.
  display_->set_buffers(buf1_, buf2_, buf_size_,
                        lvgl::Display::RenderMode::Full);

  // 4. Register VSync Callback
  esp_lcd_rgb_panel_event_callbacks_t cbs = {
      .on_vsync = on_vsync_trampoline,
  };
  esp_lcd_rgb_panel_register_event_callbacks(config.panel_handle, &cbs, this);

  // 5. Set Flush Callback
  display_->set_flush_cb(
      [this](lvgl::Display* disp, const lv_area_t* area, uint8_t* px_map) {
        this->flush_cb(disp, area, px_map);
      });

  ESP_LOGI(TAG, "Initialized ESP32-S3 RGB Display Driver (Double Buffered)");
}

Esp32RgbDisplay::~Esp32RgbDisplay() {
  if (buf1_) heap_caps_free(buf1_);
  if (buf2_) heap_caps_free(buf2_);
}

bool IRAM_ATTR Esp32RgbDisplay::on_vsync_trampoline(
    esp_lcd_panel_handle_t handle,
    const esp_lcd_rgb_panel_event_data_t* event_data, void* user_ctx) {
  auto* self = static_cast<Esp32RgbDisplay*>(user_ctx);
  // Notify LVGL that the *previous* frame is active on screen,
  // so the buffer we just flushed is now "done" (technically swapped).
  lv_display_flush_ready(self->display_->raw());
  return false;
}

void Esp32RgbDisplay::flush_cb(lvgl::Display* disp, const lv_area_t* area,
                               uint8_t* px_map) {
  // Simply update the pointer (swap) for the next frame.
  // The driver handles this via DMA shadow registers on VSync.
  esp_lcd_panel_draw_bitmap(config_.panel_handle, area->x1, area->y1,
                            area->x2 + 1, area->y2 + 1, px_map);
  // Important: DO NOT call flush_ready here!
  // We wait for VSync to ensure we don't write to the buffer being scanned.
}

}  // namespace lvgl

#endif  // __has_include("esp_lcd_panel_rgb.h")
