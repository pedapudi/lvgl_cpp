#include "port.h"

#include "esp_log.h"

static const char* TAG = "Esp32Port";

namespace lvgl {
namespace utility {

Esp32Port::Esp32Port() {
  // api_lock_ is created in init() to allow for failure handling
}

Esp32Port::~Esp32Port() {
  running_ = false;
  // Wait a bit for the task to finish naturally if it's running
  if (task_handle_) {
    vTaskDelay(pdMS_TO_TICKS(50));
    // If it hasn't nulled itself out, force delete
    if (task_handle_) {
      vTaskDelete(task_handle_);
      task_handle_ = nullptr;
    }
  }
  if (tick_timer_) {
    esp_timer_stop(tick_timer_);
    esp_timer_delete(tick_timer_);
    tick_timer_ = nullptr;
  }
  if (api_lock_) {
    vSemaphoreDelete(api_lock_);
    api_lock_ = nullptr;
  }
}

bool Esp32Port::init(const Esp32PortConfig& config) {
  if (running_) return true;

  if (!api_lock_) {
    api_lock_ = xSemaphoreCreateRecursiveMutex();
    if (!api_lock_) {
      ESP_LOGE(TAG, "Failed to create API lock");
      return false;
    }
  }

  config_ = config;

  // Initialize LVGL core if not already done
  static bool lvgl_inited = false;
  if (!lvgl_inited) {
    lv_init();
    lvgl_inited = true;
  }

  // 1. Heartbeat Timer (1ms)
  if (!tick_timer_) {
    const esp_timer_create_args_t periodic_timer_args = {
        .callback = &tick_inc_cb, .arg = nullptr, .name = "lvgl_tick"};
    if (esp_timer_create(&periodic_timer_args, &tick_timer_) != ESP_OK) {
      ESP_LOGE(TAG, "Failed to create heartbeat timer");
      return false;
    }
    if (esp_timer_start_periodic(tick_timer_, 1000) != ESP_OK) {
      ESP_LOGE(TAG, "Failed to start heartbeat timer");
      return false;
    }
  }

  // 2. LVGL Task
  running_ = true;
  BaseType_t res = xTaskCreatePinnedToCore(
      task_trampoline, "lvgl_task", config_.stack_size, this,
      config_.task_priority, &task_handle_, config_.core_affinity);

  if (res != pdPASS) {
    ESP_LOGE(TAG, "Failed to create LVGL task");
    running_ = false;
    return false;
  }

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

    if (sleep_ms == 0) sleep_ms = 1;
    if (sleep_ms > 100) sleep_ms = 100;

    // Efficiently wait for the next timer or an external notification (e.g.
    // touch/flush)
    ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(sleep_ms));
  }

  ESP_LOGI(TAG, "LVGL Task Stopping");
  task_handle_ = nullptr;
  vTaskDelete(nullptr);
}

}  // namespace utility
}  // namespace lvgl
