#include "lvgl_port.h"

#include <algorithm>

#include "esp_lcd_panel_ops.h"
#include "esp_log.h"
#include "lvgl.h"

static const char* TAG = "LvglPort";

LvglPort::LvglPort(const Config& config) : config_(config) {
  api_lock_ = xSemaphoreCreateRecursiveMutex();
}

LvglPort::~LvglPort() {
  if (tick_timer_) {
    esp_timer_stop(tick_timer_);
    esp_timer_delete(tick_timer_);
  }
  if (task_handle_) {
    vTaskDelete(task_handle_);
  }
  if (api_lock_) {
    vSemaphoreDelete(api_lock_);
  }
}

void LvglPort::init(esp_lcd_panel_handle_t panel_handle,
                    esp_lcd_panel_io_handle_t io_handle) {
  panel_handle_ = panel_handle;

  lv_init();

  lv_display_t* raw_disp = lv_display_create(config_.h_res, config_.v_res);
  if (!raw_disp) {
    ESP_LOGE(TAG, "Failed to create LVGL display");
    return;
  }
  display_ = std::make_unique<lvgl::Display>(raw_disp);
  display_->set_color_format(LV_COLOR_FORMAT_RGB565);

  size_t draw_buffer_sz =
      config_.h_res * 20 * 2;  // 2 bytes per pixel for RGB565
  draw_buf_.resize(draw_buffer_sz);

  display_->set_buffers(draw_buf_.data(), nullptr, draw_buffer_sz,
                        LV_DISPLAY_RENDER_MODE_PARTIAL);

  display_->set_flush_cb(
      [this](lvgl::Display* disp, const lv_area_t* area, uint8_t* px_map) {
        /*
         * CORE DISPLAY LOGIC: Why we process pixels here
         * ----------------------------------------------
         * Beginners often ask why we can't just send the raw buffer from LVGL.
         * There are two main reasons for this display/platform combo:
         *
         * 1. ENDIANNESS (BYTE SWAP):
         *    The ESP32-C6 is a Little-Endian architecture (least significant
         * byte first). However, SPI displays almost universally expect
         * Big-Endian (most significant byte first). Without this swap, colors
         * will look "fringy" or completely wrong because the Red and Blue bits
         * are split across the byte boundary.
         *
         * 2. COLOR INVERSION (BIT NOT):
         *    The physical display panel uses "negative logic" where a value of
         * 0 results in the brightest pixel. Since hardware inversion is
         * disabled in the driver (see gc9a01.cpp), we MUST flip all bits here
         * (~).
         *
         * WE DO BOTH CONCURRENTLY:
         * pixel = ~((byte1 << 8) | byte0)
         */
        uint16_t* buf = reinterpret_cast<uint16_t*>(px_map);
        uint32_t len = (area->x2 - area->x1 + 1) * (area->y2 - area->y1 + 1);
        for (uint32_t i = 0; i < len; i++) {
          // Swap bytes and invert bits
          buf[i] = ~((buf[i] << 8) | (buf[i] >> 8));
        }

        // Send the processed buffer to the LCD controller via SPI
        esp_lcd_panel_draw_bitmap(this->panel_handle_, area->x1, area->y1,
                                  area->x2 + 1, area->y2 + 1, px_map);
      });

  const esp_lcd_panel_io_callbacks_t cbs = {
      .on_color_trans_done = notify_flush_ready_trampoline,
  };
  esp_lcd_panel_io_register_event_callbacks(io_handle, &cbs, this);

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

  xTaskCreate(task_trampoline, "LVGL", config_.task_stack_size, this,
              config_.task_priority, &task_handle_);

  // Instantiate PointerInput
  auto ptr_input = lvgl::PointerInput::create();
  lv_indev_set_disp(ptr_input.raw(), display_->raw());  // Attach to display
  indev_ = std::make_unique<lvgl::PointerInput>(std::move(ptr_input));
}

bool LvglPort::lock(uint32_t timeout_ms) {
  return xSemaphoreTakeRecursive(api_lock_, pdMS_TO_TICKS(timeout_ms)) ==
         pdTRUE;
}

void LvglPort::unlock() { xSemaphoreGiveRecursive(api_lock_); }

bool LvglPort::notify_flush_ready_trampoline(
    esp_lcd_panel_io_handle_t io_panel, esp_lcd_panel_io_event_data_t* edata,
    void* user_ctx) {
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
    if (lock(-1)) {
      uint32_t time_till_next_ms = lv_timer_handler();
      unlock();
      time_till_next_ms =
          std::max((uint32_t)1, std::min((uint32_t)500, time_till_next_ms));
      vTaskDelay(pdMS_TO_TICKS(time_till_next_ms));
    }
  }
}
