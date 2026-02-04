#include "lvgl_cpp/display/drivers/esp32_rgb.h"

#if __has_include("esp_lcd_panel_rgb.h")
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"

static const char* TAG = "Esp32RgbDisplay";

namespace lvgl {

Esp32RgbDisplay::Esp32RgbDisplay(const Config& config) : config_(config) {
  // 1. Allocate Double Buffers in SPIRAM (DMA capable)
  // These are mandatory for the RGB LCD peripheral to scan from.
  size_t frame_bytes = config.h_res * config.v_res * sizeof(uint16_t);
  buf_size_ = frame_bytes;

  buf1_ = heap_caps_aligned_alloc(64, buf_size_,
                                  MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
  buf2_ = heap_caps_aligned_alloc(64, buf_size_,
                                  MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);

  if (!buf1_ || !buf2_) {
    ESP_LOGE(TAG, "Failed to allocate double buffers in SPIRAM");
    return;
  }

  // 2. Determine and allocate Rendering Buffer
  void* draw_buf1 = nullptr;
  void* draw_buf2 = nullptr;
  size_t draw_buf_size = 0;

  if (config_.render_mode == lvgl::Display::RenderMode::Full) {
    draw_buf1 = buf1_;
    draw_buf2 = buf2_;
    draw_buf_size = buf_size_;
  } else {
    // Partial mode: Allocate a smaller "working" buffer in fast Internal SRAM
    if (config_.partial_buf_size == 0) {
      // Default: 1/10th of the screen
      draw_buf_size = buf_size_ / 10;
    } else {
      draw_buf_size = config_.partial_buf_size;
    }
    sram_buf_ = heap_caps_aligned_alloc(64, draw_buf_size,
                                        MALLOC_CAP_INTERNAL | MALLOC_CAP_DMA);
    if (!sram_buf_) {
      ESP_LOGE(TAG, "Failed to allocate SRAM scratchpad! Falling back to Full");
      config_.render_mode = lvgl::Display::RenderMode::Full;
      draw_buf1 = buf1_;
      draw_buf2 = buf2_;
      draw_buf_size = buf_size_;
    } else {
      draw_buf1 = sram_buf_;
      draw_buf2 = nullptr;
    }
  }

  // 3. Create LVGL Display
  display_ = std::make_unique<lvgl::Display>(
      lvgl::Display::create(config.h_res, config.v_res));
  display_->set_color_format(lvgl::ColorFormat::RGB565);

  // 4. Set Buffers
  display_->set_buffers(draw_buf1, draw_buf2, draw_buf_size,
                        config_.render_mode);

  // 5. Register VSync Callback
  esp_lcd_rgb_panel_event_callbacks_t cbs = {};
  cbs.on_vsync = on_vsync_trampoline;
  esp_lcd_rgb_panel_register_event_callbacks(config.panel_handle, &cbs, this);

  // 6. Set Flush Callback
  display_->set_flush_cb(
      [this](lvgl::Display* disp, const lv_area_t* area, uint8_t* px_map) {
        this->flush_cb(area, px_map);
      });

  ESP_LOGI(TAG, "Initialized ESP32-S3 RGB Display Driver (Mode: %s)",
           config_.render_mode == lvgl::Display::RenderMode::Full ? "Full"
                                                                  : "Partial");
}

Esp32RgbDisplay::~Esp32RgbDisplay() {
  if (buf1_) heap_caps_free(buf1_);
  if (buf2_) heap_caps_free(buf2_);
  if (sram_buf_) heap_caps_free(sram_buf_);
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

void Esp32RgbDisplay::flush_cb(const lv_area_t* area, uint8_t* px_map) {
  if (config_.render_mode == lvgl::Display::RenderMode::Full) {
    // In Full mode, px_map points directly to one of our PSRAM buffers.
    // We just need to signal a swap to the hardware.
    esp_lcd_panel_draw_bitmap(config_.panel_handle, area->x1, area->y1,
                              area->x2 + 1, area->y2 + 1, px_map);
  } else {
    // In Partial mode, LVGL renders to SRAM (px_map).
    // We must copy this chunk into the currently *idle* PSRAM buffer.
    // Note: This logic assumes we know which buffer is idle.
    // On S3 RGB panels, draw_bitmap queues the buffer for NEXT VSync.
    // We'll use a simple internal pointer tracking.
    static void* current_back_buffer = buf1_;
    uint16_t* psram_dest = reinterpret_cast<uint16_t*>(current_back_buffer);

    int width = area->x2 - area->x1 + 1;
    int height = area->y2 - area->y1 + 1;
    uint16_t* sram_src = reinterpret_cast<uint16_t*>(px_map);

    // Copy row by row to the correct offset in PSRAM
    for (int y = 0; y < height; y++) {
      int dest_y = area->y1 + y;
      int dest_offset = dest_y * config_.h_res + area->x1;
      memcpy(&psram_dest[dest_offset], &sram_src[y * width],
             width * sizeof(uint16_t));
    }

    // If this is the last chunk of the frame, trigger the swap
    if (lv_display_flush_is_last(display_->raw())) {
      esp_lcd_panel_draw_bitmap(config_.panel_handle, 0, 0, config_.h_res,
                                config_.v_res, current_back_buffer);

      // Switch backbuffer for the next frame
      current_back_buffer = (current_back_buffer == buf1_) ? buf2_ : buf1_;
    } else {
      // Not the last chunk, we can call flush_ready immediately
      // because the hardware isn't using this buffer yet.
      lv_display_flush_ready(display_->raw());
    }
  }
}

}  // namespace lvgl

#endif  // __has_include("esp_lcd_panel_rgb.h")
