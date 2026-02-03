#include "esp32_spi.h"

#include <esp_heap_caps.h>
#include <esp_log.h>
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>

static const char* TAG = "Esp32Spi";

namespace lvgl {

Esp32Spi::Esp32Spi(const Config& config) : config_(config) {
  // 1. Calculate Buffer Size
  size_t px_count = config_.h_res * config_.v_res;
  buf_size_ = px_count * sizeof(uint16_t);

  ESP_LOGI(TAG, "Allocating 2x %zu bytes in SPIRAM for Native Double Buffering",
           buf_size_);

  // 2. Allocate Double Buffers (SPIRAM + DMA capable)
  // Try SPIRAM first (preferred for large framebuffers)
  buf1_ = heap_caps_aligned_alloc(64, buf_size_,
                                  MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
  if (!buf1_) {
    ESP_LOGW(TAG, "SPIRAM allocation for Buf 1 failed, trying Internal DMA");
    buf1_ = heap_caps_aligned_alloc(64, buf_size_,
                                    MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
  }

  buf2_ = heap_caps_aligned_alloc(64, buf_size_,
                                  MALLOC_CAP_SPIRAM | MALLOC_CAP_DMA);
  if (!buf2_) {
    ESP_LOGW(TAG, "SPIRAM allocation for Buf 2 failed, trying Internal DMA");
    buf2_ = heap_caps_aligned_alloc(64, buf_size_,
                                    MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL);
  }

  if (!buf1_ || !buf2_) {
    ESP_LOGE(TAG, "Failed to allocate SPIRAM buffers!");
    abort();
  }

  // 3. Register DMA completion callback
  esp_lcd_panel_io_callbacks_t cbs = {
      .on_color_trans_done = on_color_trans_done_trampoline,
  };
  esp_lcd_panel_io_register_event_callbacks(config_.io_handle, &cbs, this);

  // 4. Create LVGL Display
  display_ = std::make_unique<lvgl::Display>(config_.h_res, config_.v_res);

  // 5. Configure Buffers (Full Double Buffering)
  display_->set_buffers(buf1_, buf2_, buf_size_,
                        lvgl::Display::RenderMode::Full);

  // 6. Select Flush Implementation
  if (!config_.swap_bytes) {
    selected_flush_ = &Esp32Spi::flush_no_processing;
  } else if (config_.invert_colors) {
    selected_flush_ = &Esp32Spi::flush_swap_invert;
  } else {
    selected_flush_ = &Esp32Spi::flush_swap;
  }

  // 7. Set Flush Callback
  display_->set_flush_cb(
      [this](lvgl::Display* d, const lv_area_t* area, uint8_t* px_map) {
        this->flush_cb(d, area, px_map);
      });
}

Esp32Spi::~Esp32Spi() {
  if (buf1_) heap_caps_free(buf1_);
  if (buf2_) heap_caps_free(buf2_);
}

bool IRAM_ATTR Esp32Spi::on_color_trans_done_trampoline(
    esp_lcd_panel_io_handle_t panel_io, esp_lcd_panel_io_event_data_t* edata,
    void* user_ctx) {
  auto* self = static_cast<Esp32Spi*>(user_ctx);
  // Notify LVGL that the *previous* frame is done and we are ready for the next
  if (self->display_) {
    lv_display_flush_ready(self->display_->raw());
  }
  return false;
}

void Esp32Spi::flush_cb(lvgl::Display* disp, const lv_area_t* area,
                        uint8_t* px_map) {
  // Call the pre-selected optimized implementation
  (this->*selected_flush_)(area, px_map);
}

void Esp32Spi::flush_no_processing(const lv_area_t* area, uint8_t* px_map) {
  esp_lcd_panel_draw_bitmap(config_.panel_handle, area->x1, area->y1,
                            area->x2 + 1, area->y2 + 1, px_map);
}

void Esp32Spi::flush_swap(const lv_area_t* area, uint8_t* px_map) {
  uint16_t* buf = reinterpret_cast<uint16_t*>(px_map);
  uint32_t len = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);

  // Aligned: Use 32-bit access (process 2 pixels at once)
  uint32_t* buf32 = reinterpret_cast<uint32_t*>(buf);
  uint32_t len32 = len / 2;
  uint32_t i = 0;

  // Unroll loop for 8x32-bit operations (16 pixels) per iteration
  while (i + 8 <= len32) {
    uint32_t v0 = buf32[i];
    uint32_t v1 = buf32[i + 1];
    uint32_t v2 = buf32[i + 2];
    uint32_t v3 = buf32[i + 3];
    uint32_t v4 = buf32[i + 4];
    uint32_t v5 = buf32[i + 5];
    uint32_t v6 = buf32[i + 6];
    uint32_t v7 = buf32[i + 7];

    auto swap = [](uint32_t v) {
      return ((v & 0xFF00FF00) >> 8) | ((v & 0x00FF00FF) << 8);
    };

    buf32[i] = swap(v0);
    buf32[i + 1] = swap(v1);
    buf32[i + 2] = swap(v2);
    buf32[i + 3] = swap(v3);
    buf32[i + 4] = swap(v4);
    buf32[i + 5] = swap(v5);
    buf32[i + 6] = swap(v6);
    buf32[i + 7] = swap(v7);
    i += 8;
  }

  while (i < len32) {
    uint32_t v = buf32[i];
    buf32[i] = ((v & 0xFF00FF00) >> 8) | ((v & 0x00FF00FF) << 8);
    i++;
  }

  if (len % 2) {
    uint16_t v = buf[len - 1];
#if defined(__XTENSA__)
    buf[len - 1] = __builtin_bswap16(v);
#else
    buf[len - 1] = (v << 8) | (v >> 8);
#endif
  }

  esp_lcd_panel_draw_bitmap(config_.panel_handle, area->x1, area->y1,
                            area->x2 + 1, area->y2 + 1, px_map);
}

void Esp32Spi::flush_swap_invert(const lv_area_t* area, uint8_t* px_map) {
  uint16_t* buf = reinterpret_cast<uint16_t*>(px_map);
  uint32_t len = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);

  uint32_t* buf32 = reinterpret_cast<uint32_t*>(buf);
  uint32_t len32 = len / 2;
  uint32_t i = 0;

  while (i + 8 <= len32) {
    uint32_t v0 = buf32[i];
    uint32_t v1 = buf32[i + 1];
    uint32_t v2 = buf32[i + 2];
    uint32_t v3 = buf32[i + 3];
    uint32_t v4 = buf32[i + 4];
    uint32_t v5 = buf32[i + 5];
    uint32_t v6 = buf32[i + 6];
    uint32_t v7 = buf32[i + 7];

    auto swap_inv = [](uint32_t v) {
      uint32_t s = ((v & 0xFF00FF00) >> 8) | ((v & 0x00FF00FF) << 8);
      return ~s;
    };

    buf32[i] = swap_inv(v0);
    buf32[i + 1] = swap_inv(v1);
    buf32[i + 2] = swap_inv(v2);
    buf32[i + 3] = swap_inv(v3);
    buf32[i + 4] = swap_inv(v4);
    buf32[i + 5] = swap_inv(v5);
    buf32[i + 6] = swap_inv(v6);
    buf32[i + 7] = swap_inv(v7);
    i += 8;
  }

  while (i < len32) {
    uint32_t v = buf32[i];
    uint32_t s = ((v & 0xFF00FF00) >> 8) | ((v & 0x00FF00FF) << 8);
    buf32[i] = ~s;
    i++;
  }

  if (len % 2) {
    uint16_t v = buf[len - 1];
#if defined(__XTENSA__)
    buf[len - 1] = ~__builtin_bswap16(v);
#else
    uint16_t s = (v << 8) | (v >> 8);
    buf[len - 1] = ~s;
#endif
  }

  esp_lcd_panel_draw_bitmap(config_.panel_handle, area->x1, area->y1,
                            area->x2 + 1, area->y2 + 1, px_map);
}

}  // namespace lvgl
