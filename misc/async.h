#ifndef LVGL_CPP_MISC_ASYNC_H_
#define LVGL_CPP_MISC_ASYNC_H_

#include <atomic>
#include <functional>

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

/**
 * @brief Handle for a cancellable async call.
 *
 * When destroyed, cancels the pending async call if not yet executed.
 * Move-only type to prevent double-cancellation.
 */
class AsyncHandle {
 public:
  AsyncHandle();
  ~AsyncHandle();

  AsyncHandle(const AsyncHandle&) = delete;
  AsyncHandle& operator=(const AsyncHandle&) = delete;
  AsyncHandle(AsyncHandle&& other) noexcept;
  AsyncHandle& operator=(AsyncHandle&& other) noexcept;

  /**
   * @brief Cancel the pending async call.
   * @return true if cancelled, false if already executed or invalid.
   */
  bool cancel();

  /**
   * @brief Check if this handle is valid (has a pending call).
   */
  bool valid() const;

  /**
   * @brief Release ownership without cancelling.
   * The async call will still execute but cannot be cancelled.
   */
  void release();

 private:
  struct CallbackData;
  CallbackData* data_;

  explicit AsyncHandle(CallbackData* data);

  // Allow proxy functions access to CallbackData
  friend void async_fire_and_forget_proxy(void* user_data);
  friend void async_cancellable_proxy(void* user_data);
  friend class Async;
};

/**
 * @brief Utility class for deferred/asynchronous execution.
 *
 * All methods are static since async calls are global operations.
 * This wraps LVGL's lv_async_call() and lv_async_call_cancel() APIs.
 */
class Async {
 public:
  /**
   * @brief Schedule a callback for deferred execution (fire-and-forget).
   *
   * The callback will execute on the next lv_timer_handler() cycle.
   * This is safe for thread-to-UI communication.
   *
   * @param callback The function to execute asynchronously.
   * @return LV_RESULT_OK on success, LV_RESULT_INVALID on failure.
   *
   * @example
   * lvgl::Async::call([this]() { this->update_ui(); });
   */
  static lv_result_t call(std::function<void()> callback);

  /**
   * @brief Schedule a cancellable callback for deferred execution.
   *
   * Returns a handle that can be used to cancel the pending call.
   * If the handle is destroyed before execution, the call is cancelled.
   *
   * @param callback The function to execute asynchronously.
   * @return An AsyncHandle for cancellation.
   *
   * @example
   * auto handle = lvgl::Async::call_cancellable([this]() { this->do_work(); });
   * handle.cancel();  // Cancel if needed
   */
  static AsyncHandle call_cancellable(std::function<void()> callback);

 private:
  Async() = delete;  // Static-only class
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ASYNC_H_
