#include "lvgl_cpp/display/drivers/esp32_rgb.h"

#if __has_include("esp_lcd_panel_rgb.h")
#include "esp_async_memcpy.h"
#include "esp_heap_caps.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"
#include "esp_timer.h"

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
  esp_lcd_rgb_panel_event_callbacks_t cbs;
  memset(&cbs, 0, sizeof(cbs));
  cbs.on_vsync = on_vsync_trampoline;

  esp_lcd_rgb_panel_register_event_callbacks(config.panel_handle, &cbs, this);

  // 6. Set Flush Callback
  display_->set_flush_cb(
      [this](lvgl::Display* disp, const lv_area_t* area, uint8_t* px_map) {
        this->flush_cb(area, px_map);
      });

  // 7. Initialize GDMA for M2M copies if in Partial mode
  if (config_.render_mode == lvgl::Display::RenderMode::Partial) {
    async_memcpy_config_t m2m_config;
    memset(&m2m_config, 0, sizeof(m2m_config));
    m2m_config.backlog = 128;  // Support up to 128 sequential row copies

    esp_async_memcpy_install(&m2m_config,
                             reinterpret_cast<async_memcpy_handle_t*>(&m2m_));
    current_back_buffer_ = buf1_;
  }

  ESP_LOGI(TAG, "Initialized ESP32-S3 RGB Display Driver (Mode: %s)",
           config_.render_mode == lvgl::Display::RenderMode::Full ? "Full"
                                                                  : "Partial");
}

Esp32RgbDisplay::~Esp32RgbDisplay() {
  if (m2m_) {
    esp_async_memcpy_uninstall(static_cast<async_memcpy_handle_t>(m2m_));
  }
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
    esp_lcd_panel_draw_bitmap(config_.panel_handle, area->x1, area->y1,
                              area->x2 + 1, area->y2 + 1, px_map);
  } else {
    // In Partial mode, LVGL renders to SRAM (px_map).
    // We offload the copy to PSRAM using GDMA.
    uint16_t* psram_dest = reinterpret_cast<uint16_t*>(current_back_buffer_);
    uint16_t* sram_src = reinterpret_cast<uint16_t*>(px_map);
    int width = area->x2 - area->x1 + 1;
    int height = area->y2 - area->y1 + 1;

    this->is_last_chunk_ = lv_display_flush_is_last(display_->raw());

    // Queue row-by-row copies
    for (int y = 0; y < height; y++) {
      int dest_y = area->y1 + y;
      int dest_offset = dest_y * config_.h_res + area->x1;

      // The last row of the chunk triggers the completion callback
      bool is_last_row = (y == height - 1);
      esp_async_memcpy(static_cast<async_memcpy_handle_t>(m2m_),
                       &psram_dest[dest_offset], &sram_src[y * width],
                       width * sizeof(uint16_t),
                       is_last_row ? on_m2m_done_trampoline : nullptr, this);
    }
  }
}

bool IRAM_ATTR Esp32RgbDisplay::on_m2m_done_trampoline(
    async_memcpy_handle_t m2m, async_memcpy_event_t* event, void* user_ctx) {
  auto* self = static_cast<Esp32RgbDisplay*>(user_ctx);

  if (self->is_last_chunk_) {
    // End of frame: Trigger the hardware swap to the buffer we just filled.
    esp_lcd_panel_draw_bitmap(self->config_.panel_handle, 0, 0,
                              self->config_.h_res, self->config_.v_res,
                              self->current_back_buffer_);

    // Switch backbuffer for the next frame.
    // Note: on_vsync_trampoline will call flush_ready when this buffer
    // actually becomes visible.
    self->current_back_buffer_ =
        (self->current_back_buffer_ == self->buf1_) ? self->buf2_ : self->buf1_;
  } else {
    // Just an intermediate chunk: SRAM is now copied, so it's free for LVGL.
    lv_display_flush_ready(self->display_->raw());
  }

  return false;
}

}  // namespace lvgl

#endif  // __has_include("esp_lcd_panel_rgb.h")
