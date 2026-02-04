#ifndef LVGL_CPP_UTILITY_PORTABLE_ESP32_PORT_H_
#define LVGL_CPP_UTILITY_PORTABLE_ESP32_PORT_H_

#include <atomic>
#include <cstdint>
#include <memory>

#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "lvgl.h"

namespace lvgl {
namespace utility {

/**
 * @brief Configuration for the ESP32 Port utility.
 */
struct Esp32PortConfig {
  uint32_t h_res = 240;
  uint32_t v_res = 240;
  size_t stack_size = 32 * 1024;
  UBaseType_t task_priority = 5;
  BaseType_t core_affinity = 1;  ///< Set to tskNO_AFFINITY for no pinning
};

/**
 * @brief A high-level utility to manage the LVGL environment on ESP32.
 * Handles the FreeRTOS task, heartbeat timer, and thread synchronization.
 */
class Esp32Port {
 public:
  Esp32Port();
  ~Esp32Port();

  /**
   * @brief Initialize the port with hardware handles.
   * @param config Port configuration.
   * @return true if initialized successfully.
   */
  bool init(const Esp32PortConfig& config);

  /**
   * @brief Execute a function with the LVGL API lock held.
   */
  template <typename F>
  void lock(F&& func) {
    if (api_lock_ &&
        xSemaphoreTakeRecursive(api_lock_, portMAX_DELAY) == pdTRUE) {
      func();
      xSemaphoreGiveRecursive(api_lock_);
    }
  }

  /**
   * @brief Get the underlying API lock (mutex).
   * Useful for manual lock/unlock in legacy code.
   */
  SemaphoreHandle_t get_lock() const { return api_lock_; }

  /**
   * @brief Notify the LVGL task to wake up immediately.
   */
  void notify() {
    if (task_handle_) {
      xTaskNotifyGive(task_handle_);
    }
  }

  /**
   * @brief Notify the LVGL task from an ISR.
   */
  void notify_from_isr() {
    if (task_handle_) {
      BaseType_t high_task_wakeup = pdFALSE;
      vTaskNotifyGiveFromISR(task_handle_, &high_task_wakeup);
      if (high_task_wakeup) {
        portYIELD_FROM_ISR();
      }
    }
  }

 private:
  static void tick_inc_cb(void* arg);
  static void task_trampoline(void* arg);
  void task_loop();

  Esp32PortConfig config_;
  SemaphoreHandle_t api_lock_ = nullptr;
  TaskHandle_t task_handle_ = nullptr;
  esp_timer_handle_t tick_timer_ = nullptr;
  std::atomic<bool> running_{false};
};

}  // namespace utility
}  // namespace lvgl

#endif  // LVGL_CPP_UTILITY_PORTABLE_ESP32_PORT_H_
