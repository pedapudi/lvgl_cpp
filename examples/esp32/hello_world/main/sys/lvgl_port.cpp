#include "lvgl_port.h"
#include "esp_check.h"
#include "esp_log.h"
#include <algorithm> // for std::max, std::min
#include <unistd.h>

static const char *TAG = "LvglPort";

LvglPort::LvglPort(const Config &config) : config_(config) {
  api_lock_ = xSemaphoreCreateRecursiveMutex();
}

void LvglPort::init(esp_lcd_panel_handle_t panel_handle,
                    esp_lcd_panel_io_handle_t io_handle) {
  panel_handle_ = panel_handle;

  ESP_LOGI(TAG, "Initialize LVGL");
  lv_init();

  lv_display_t *raw_disp = lv_display_create(config_.h_res, config_.v_res);
  display_ = std::make_unique<lvgl::Display>(raw_disp);

  // Allocate draw buffer (monochrome needs extra for palette)
  size_t draw_buffer_sz =
      config_.h_res * config_.v_res / 8 + 8; // +8 for palette
  draw_buffer_.resize(draw_buffer_sz);
  oled_buffer_.resize(config_.h_res * config_.v_res / 8);

  display_->set_color_format(LV_COLOR_FORMAT_I1);
  display_->set_buffers(draw_buffer_.data(), nullptr, draw_buffer_sz,
                        LV_DISPLAY_RENDER_MODE_FULL);

  // Use lambda to capture 'this' and call member function
  display_->set_flush_cb(
      [this](lvgl::Display *disp, const lv_area_t *area, uint8_t *px_map) {
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
  ESP_ERROR_CHECK(
      esp_timer_start_periodic(tick_timer_, config_.tick_period_ms * 1000));

  // Task
  xTaskCreate(task_trampoline, "LVGL", config_.task_stack_size, this,
              config_.task_priority, &task_handle_);
}

bool LvglPort::lock(uint32_t timeout_ms) {
  return xSemaphoreTakeRecursive(api_lock_, pdMS_TO_TICKS(timeout_ms)) ==
         pdTRUE;
}

void LvglPort::unlock() { xSemaphoreGiveRecursive(api_lock_); }

void LvglPort::flush_cb(lvgl::Display *disp, const lv_area_t *area,
                        uint8_t *px_map) {
  // Skip palette
  px_map += 8;

  uint16_t hor_res = config_.h_res;
  int x1 = area->x1;
  int x2 = area->x2;
  int y1 = area->y1;
  int y2 = area->y2;

  // static std::vector<uint8_t> s_oled_buffer(config_.h_res * config_.v_res / 8);

  for (int y = y1; y <= y2; y++) {
    for (int x = x1; x <= x2; x++) {
      // Logic from main.cpp
      bool chroma_color =
          (px_map[(hor_res >> 3) * y + (x >> 3)] & 1 << (7 - x % 8));
      uint8_t *buf = oled_buffer_.data() + hor_res * (y >> 3) + (x);
      if (chroma_color) {
        (*buf) &= ~(1 << (y % 8));
      } else {
        (*buf) |= (1 << (y % 8));
      }
    }
  }

  esp_lcd_panel_draw_bitmap(panel_handle_, x1, y1, x2 + 1, y2 + 1,
                            oled_buffer_.data());
}

bool LvglPort::notify_flush_ready_trampoline(
    esp_lcd_panel_io_handle_t io_panel, esp_lcd_panel_io_event_data_t *edata,
    void *user_ctx) {
  auto *self = static_cast<LvglPort *>(user_ctx);
  self->display_->flush_ready();
  return false;
}

void LvglPort::tick_increment_trampoline(void *arg) {
  auto *self = static_cast<LvglPort *>(arg);
  lv_tick_inc(self->config_.tick_period_ms);
}

void LvglPort::task_trampoline(void *arg) {
  auto *self = static_cast<LvglPort *>(arg);
  self->task_loop();
}

void LvglPort::task_loop() {
  ESP_LOGI(TAG, "Starting LVGL task");
  while (1) {
    lock(-1); // Wait forever
    uint32_t time_till_next_ms = lv_timer_handler();
    unlock();

    time_till_next_ms = std::max(time_till_next_ms, (uint32_t)1);
    time_till_next_ms = std::min(time_till_next_ms, (uint32_t)500);

    vTaskDelay(pdMS_TO_TICKS(time_till_next_ms));
  }
}
