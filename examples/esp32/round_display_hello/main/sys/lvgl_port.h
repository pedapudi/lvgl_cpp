#pragma once

#include <memory>
#include <vector>

#include "display/display.h"
#include "esp_lcd_panel_io.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "lvgl.h"
#include "lvgl_cpp/indev/input_device.h"
#include "lvgl_cpp/indev/pointer_input.h"
#include "lvgl_cpp/lvgl_cpp.h"
#include "lvgl_cpp/misc/enums.h"

#if __has_include("esp_lcd_panel_rgb.h")
#include "esp_lcd_panel_rgb.h"
#endif

class LvglPort {
 public:
  struct Config {
    int h_res = 240;
    int v_res = 240;
    int tick_period_ms = 5;
    int task_stack_size = 4096;
    int task_priority = 5;
  };

  explicit LvglPort(const Config& config);
  ~LvglPort();

  void init(esp_lcd_panel_handle_t panel_handle,
            esp_lcd_panel_io_handle_t io_handle);

  bool lock(uint32_t timeout_ms);
  void unlock();

  lvgl::Display* get_display() { return display_.get(); }

  template <typename T>
  void register_touch_driver(T* driver) {
    if (indev_) {
      // Use IndevData& signature if available, or fallback to raw pointer if
      // wrapper expects it. Based on input_device.h, set_read_cb takes
      // std::function<void(IndevData&)>
      indev_->set_read_cb([driver](lvgl::IndevData& data) {
        uint16_t x = 0, y = 0;
        bool pressed = false;
        driver->read(&x, &y, &pressed);

        if (pressed) {
          ESP_LOGI("LvglPort", "Touch: x=%d, y=%d", x, y);
          data.set_state(lvgl::IndevState::Pressed);
          data.set_point(x, y);
        } else {
          data.set_state(lvgl::IndevState::Released);
        }
      });
    }
  }

 private:
  static bool notify_flush_ready_trampoline(
      esp_lcd_panel_io_handle_t io_panel, esp_lcd_panel_io_event_data_t* edata,
      void* user_ctx);
  static void tick_increment_trampoline(void* arg);
  static void task_trampoline(void* arg);

  void task_loop();

  Config config_;
  SemaphoreHandle_t api_lock_;
  std::unique_ptr<lvgl::Display> display_;
  std::unique_ptr<lvgl::PointerInput> indev_;
  esp_timer_handle_t tick_timer_;
  TaskHandle_t task_handle_;
  esp_lcd_panel_handle_t panel_handle_;
  std::vector<uint8_t> draw_buf_;
};

#include "lvgl_cpp/display/drivers/esp32_rgb.h"

// Note: Esp32RgbDisplay is now imported from
// <lvgl_cpp/display/drivers/esp32_rgb.h>
