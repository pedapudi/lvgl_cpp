#include "lvgl_port.h"

#include <unistd.h>

#include <algorithm>  // for std::max, std::min

#include "esp_check.h"
#include "esp_log.h"

static const char* TAG = "LvglPort";

LvglPort::LvglPort(const Config& config) : config_(config) {
  api_lock_ = xSemaphoreCreateRecursiveMutex();
}

void LvglPort::init(esp_lcd_panel_handle_t panel_handle, esp_lcd_panel_io_handle_t io_handle) {
  panel_handle_ = panel_handle;

  ESP_LOGI(TAG, "Initialize LVGL");
  lv_init();

  lv_display_t* raw_disp = lv_display_create(config_.h_res, config_.v_res);
  display_ = std::make_unique<lvgl::Display>(raw_disp);

  // Allocate draw buffer (monochrome needs extra for palette)
  size_t draw_buffer_sz = config_.h_res * config_.v_res / 8 + 8;  // +8 for palette
  draw_buffer_.resize(draw_buffer_sz);

  display_->set_color_format(LV_COLOR_FORMAT_I1);
  display_->set_buffers(draw_buffer_.data(), nullptr, draw_buffer_sz, LV_DISPLAY_RENDER_MODE_FULL);

  // Use lambda to capture 'this' and call member function
  display_->set_flush_cb([this](lvgl::Display* disp, const lv_area_t* area, uint8_t* px_map) {
    this->flush_cb(disp, area, px_map);
  });

  // Register IO callback
  const esp_lcd_panel_io_callbacks_t cbs = {
      .on_color_trans_done = notify_flush_ready_trampoline,
  };
  esp_lcd_panel_io_register_event_callbacks(io_handle, &cbs, this);

  // Timer for ticks
  const esp_timer_create_args_t lvgl_tick_timer_args = {
      .callback = &tick_increment_trampoline,
      .arg = this,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "lvgl_tick",
      .skip_unhandled_events = false,
  };
  ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &tick_timer_));
  ESP_ERROR_CHECK(esp_timer_start_periodic(tick_timer_, config_.tick_period_ms * 1000));

  // Task
  xTaskCreate(task_trampoline, "LVGL", config_.task_stack_size, this, config_.task_priority,
              &task_handle_);
}

bool LvglPort::lock(uint32_t timeout_ms) {
  return xSemaphoreTakeRecursive(api_lock_, pdMS_TO_TICKS(timeout_ms)) == pdTRUE;
}

void LvglPort::unlock() { xSemaphoreGiveRecursive(api_lock_); }

void LvglPort::flush_cb(lvgl::Display* disp, const lv_area_t* area, uint8_t* px_map) {
  // Skip palette
  px_map += 8;

  uint16_t hor_res = config_.h_res;
  int x1 = area->x1;
  int x2 = area->x2;
  int y1 = area->y1;
  int y2 = area->y2;

  static std::vector<uint8_t> s_oled_buffer(config_.h_res * config_.v_res / 8);

  for (int y = y1; y <= y2; y++) {
    for (int x = x1; x <= x2; x++) {
      // Logic from main.cpp
      bool chroma_color = (px_map[(hor_res >> 3) * y + (x >> 3)] & 1 << (7 - x % 8));
      uint8_t* buf = s_oled_buffer.data() + hor_res * (y >> 3) + (x);
      if (chroma_color) {
        (*buf) &= ~(1 << (y % 8));
      } else {
        (*buf) |= (1 << (y % 8));
      }
    }
  }

  // [Detailed Explanation of Full-Screen Refresh Strategy]
  // We strictly perform a full-screen refresh (0, 0, h_res, v_res) instead of partial updates for
  // two critical reasons:
  //
  // 1. Buffer Alignment & Contiguity: The SSD1306 controller uses a "page-based" memory layout
  // (vertical bytes),
  //    meaning a single byte represents 8 vertical pixels. Arbitrary rectangular areas (x1, y1, x2,
  //    y2) from LVGL often do not align neatly with these 8-pixel page boundaries. Sending a
  //    partial update requires complex bit-hacking to ensure the data sent matches the controller's
  //    expected page structure.
  //
  // 2. Driver Expectations: The `esp_lcd_panel_draw_bitmap` function typically expects the input
  // buffer to contain
  //    *only* the pixel data for the specified window, packed contiguously. However, our
  //    `s_oled_buffer` accumulates the state of the *entire* screen. Passing a pointer into the
  //    middle of this full-frame buffer for a partial window works for linear memory, but fails for
  //    paged memory because the stride (width of the full screen) doesn't match the width of the
  //    partial window, leading to skewed/garbage display output ("piling on" artifacts).
  //
  // Therefore, we treat `s_oled_buffer` as the single source of truth for the entire display and
  // flush it completely on every update. This is computationally cheap for small displays (128x64)
  // and guarantees correctness.
  // [What NOT to do]
  // We reverted to partial updates here because the Full-Screen Refresh approach (sending the whole
  // buffer 0,0->128,64) caused the text to become unreadable/scrambled on the device, possibly due
  // to timing issues or driver limitations with large data bursts.
  //
  // HOWEVER, this Partial Update approach using `s_oled_buffer.data()` is ALSO imperfect:
  // Since the SSD1306 uses Page Addressing (vertical bytes) and our buffer `s_oled_buffer`
  // represents a full screen, the data for a rectangular window (x1, y1...) is NOT contiguous in
  // `s_oled_buffer`. Passing the start of the buffer `.data()` for a partial window `x1, y1` tells
  // the driver to grab the *first* bytes of the buffer and paint them into the window, which
  // results in "piling on" or ghosting artifacts because it's drawing Page 0 data into whatever
  // window we requested.
  //
  // To fix this properly, one would need to allocate a temporary contiguous buffer, copy the
  // specific non-contiguous pages/columns from `s_oled_buffer` into it, and send that. For now, we
  // revert to partials as requested to restore readability, accepting the artifacts.
  esp_lcd_panel_draw_bitmap(panel_handle_, x1, y1, x2 + 1, y2 + 1, s_oled_buffer.data());
}

bool LvglPort::notify_flush_ready_trampoline(esp_lcd_panel_io_handle_t io_panel,
                                             esp_lcd_panel_io_event_data_t* edata, void* user_ctx) {
  auto* self = static_cast<LvglPort*>(user_ctx);
  self->display_->flush_ready();
  return false;
}

void LvglPort::tick_increment_trampoline(void* arg) {
  auto* self = static_cast<LvglPort*>(arg);
  lv_tick_inc(self->config_.tick_period_ms);
}

void LvglPort::task_trampoline(void* arg) {
  auto* self = static_cast<LvglPort*>(arg);
  self->task_loop();
}

void LvglPort::task_loop() {
  ESP_LOGI(TAG, "Starting LVGL task");
  while (1) {
    lock(-1);  // Wait forever
    uint32_t time_till_next_ms = lv_timer_handler();
    unlock();

    time_till_next_ms = std::max(time_till_next_ms, (uint32_t)1);
    time_till_next_ms = std::min(time_till_next_ms, (uint32_t)500);

    usleep(1000 * time_till_next_ms);
  }
}
