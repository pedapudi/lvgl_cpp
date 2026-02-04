#include "port.h"

#include "esp_log.h"

static const char* TAG = "Esp32Port";

namespace lvgl {
namespace utility {

Esp32Port::Esp32Port() { api_lock_ = xSemaphoreCreateRecursiveMutex(); }

Esp32Port::~Esp32Port() {
  running_ = false;
  if (task_handle_) {
    vTaskDelete(task_handle_);
  }
  if (tick_timer_) {
    esp_timer_stop(tick_timer_);
    esp_timer_delete(tick_timer_);
  }
  if (api_lock_) {
    vSemaphoreDelete(api_lock_);
  }
}

bool Esp32Port::init(const Esp32PortConfig& config) {
  config_ = config;

  lv_init();

  // 1. Heartbeat Timer (1ms)
  const esp_timer_create_args_t periodic_timer_args = {.callback = &tick_inc_cb,
                                                       .name = "lvgl_tick"};
  esp_timer_create(&periodic_timer_args, &tick_timer_);
  esp_timer_start_periodic(tick_timer_, 1000);

  // 2. LVGL Task
  running_ = true;
  xTaskCreatePinnedToCore(task_trampoline, "lvgl_task", config_.stack_size,
                          this, config_.task_priority, &task_handle_,
                          config_.core_affinity);

  return true;
}

void Esp32Port::tick_inc_cb(void* arg) { lv_tick_inc(1); }

void Esp32Port::task_trampoline(void* arg) {
  static_cast<Esp32Port*>(arg)->task_loop();
}

void Esp32Port::task_loop() {
  ESP_LOGI(TAG, "LVGL Task Started on Core %d", xPortGetCoreID());

  while (running_) {
    uint32_t sleep_ms = 10;
    if (xSemaphoreTakeRecursive(api_lock_, portMAX_DELAY) == pdTRUE) {
      sleep_ms = lv_timer_handler();
      xSemaphoreGiveRecursive(api_lock_);
    }

    if (sleep_ms < 1) sleep_ms = 1;
    if (sleep_ms > 100) sleep_ms = 100;
    vTaskDelay(pdMS_TO_TICKS(sleep_ms));
  }

  vTaskDelete(nullptr);
}

}  // namespace utility
}  // namespace lvgl
