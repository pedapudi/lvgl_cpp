#pragma once

#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "lvgl_cpp/lvgl_cpp.h"
#include <memory>
#include <vector>

class LvglPort {
public:
  struct Config {
    uint16_t h_res = 128;
    uint16_t v_res = 64;
    uint32_t task_stack_size = 4096;
    int task_priority = 2;
    int task_core = 0;
    uint32_t tick_period_ms = 5;
  };

  explicit LvglPort(const Config &config);
  ~LvglPort() = default;

  void init(esp_lcd_panel_handle_t panel_handle,
            esp_lcd_panel_io_handle_t io_handle);
  bool lock(uint32_t timeout_ms = 0);
  void unlock();

  lvgl::Display *get_display() { return display_.get(); }

private:
  void flush_cb(lvgl::Display *disp, const lv_area_t *area, uint8_t *px_map);
  static bool
  notify_flush_ready_trampoline(esp_lcd_panel_io_handle_t io_panel,
                                esp_lcd_panel_io_event_data_t *edata,
                                void *user_ctx);
  static void tick_increment_trampoline(void *arg);
  static void task_trampoline(void *arg);

  void task_loop();

  Config config_;
  std::unique_ptr<lvgl::Display> display_;
  std::vector<uint8_t> draw_buffer_;
  esp_lcd_panel_handle_t panel_handle_ = nullptr;

  SemaphoreHandle_t api_lock_ = nullptr;
  esp_timer_handle_t tick_timer_ = nullptr;
  TaskHandle_t task_handle_ = nullptr;
};
