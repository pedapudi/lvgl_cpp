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

  // 6. Set Flush Callback
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
  // Optional: Byte Swapping / Color Inversion
  // -----------------------------------------
  // High-performance panels often need Big-Endian data (RGB565).
  // If native swapping isn't available, we use SIMD intrinsics here.

  if (config_.swap_bytes) {
    uint16_t* buf = reinterpret_cast<uint16_t*>(px_map);
    uint32_t len = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);

#if defined(__XTENSA__)
    // Xtensa SIMD Optimization
    if (config_.invert_colors) {
      // Swap + Invert
      while (len >= 8) {
        buf[0] = ~__builtin_bswap16(buf[0]);
        buf[1] = ~__builtin_bswap16(buf[1]);
        buf[2] = ~__builtin_bswap16(buf[2]);
        buf[3] = ~__builtin_bswap16(buf[3]);
        buf[4] = ~__builtin_bswap16(buf[4]);
        buf[5] = ~__builtin_bswap16(buf[5]);
        buf[6] = ~__builtin_bswap16(buf[6]);
        buf[7] = ~__builtin_bswap16(buf[7]);
        buf += 8;
        len -= 8;
      }
    } else {
      // Just Swap
      while (len >= 8) {
        buf[0] = __builtin_bswap16(buf[0]);
        buf[1] = __builtin_bswap16(buf[1]);
        buf[2] = __builtin_bswap16(buf[2]);
        buf[3] = __builtin_bswap16(buf[3]);
        buf[4] = __builtin_bswap16(buf[4]);
        buf[5] = __builtin_bswap16(buf[5]);
        buf[6] = __builtin_bswap16(buf[6]);
        buf[7] = __builtin_bswap16(buf[7]);
        buf += 8;
        len -= 8;
      }
    }
#endif  // __XTENSA__

    // Fallback or cleanup loop
    for (uint32_t i = 0; i < len; i++) {
      uint16_t val = buf[i];
#if defined(__XTENSA__)
      buf[i] = config_.invert_colors ? ~__builtin_bswap16(val)
                                     : __builtin_bswap16(val);
#else
      // Standard C fallback
      val = (val << 8) | (val >> 8);
      buf[i] = config_.invert_colors ? ~val : val;
#endif
    }
  }

  // DMA Transfer
  // Pass the buffer directly to the SPI controller.
  esp_lcd_panel_draw_bitmap(config_.panel_handle, area->x1, area->y1,
                            area->x2 + 1, area->y2 + 1, px_map);
}

}  // namespace lvgl
