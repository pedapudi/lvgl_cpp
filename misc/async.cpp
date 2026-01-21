#include "async.h"

#include <utility>

namespace lvgl {

// Internal callback data structure
struct AsyncHandle::CallbackData {
  std::function<void()> callback;
  std::atomic<bool> executed{false};
  std::atomic<bool> cancelled{false};
  std::atomic<bool> owned{true};  // Whether AsyncHandle owns this data
};

// C trampoline for fire-and-forget calls
void async_fire_and_forget_proxy(void* user_data) {
  auto* data = static_cast<AsyncHandle::CallbackData*>(user_data);
  if (data && data->callback) {
    data->callback();
  }
  delete data;  // Always delete for fire-and-forget
}

// C trampoline for cancellable calls
void async_cancellable_proxy(void* user_data) {
  auto* data = static_cast<AsyncHandle::CallbackData*>(user_data);
  if (!data) return;

  data->executed.store(true);

  if (!data->cancelled.load() && data->callback) {
    data->callback();
  }

  // If handle released ownership, we delete here
  if (!data->owned.load()) {
    delete data;
  }
}

// AsyncHandle implementation

AsyncHandle::AsyncHandle() : data_(nullptr) {}

AsyncHandle::AsyncHandle(CallbackData* data) : data_(data) {}

AsyncHandle::~AsyncHandle() {
  if (data_ && data_->owned.load()) {
    cancel();
    // If already executed, data was not deleted by cancel, so delete here
    if (data_->executed.load()) {
      delete data_;
    }
    // If cancel succeeded and not executed, lv_async_call_cancel deleted the
    // timer but we still own the data
    else {
      delete data_;
    }
  }
}

AsyncHandle::AsyncHandle(AsyncHandle&& other) noexcept : data_(other.data_) {
  other.data_ = nullptr;
}

AsyncHandle& AsyncHandle::operator=(AsyncHandle&& other) noexcept {
  if (this != &other) {
    // Clean up current data if owned
    if (data_ && data_->owned.load()) {
      cancel();
      delete data_;
    }
    data_ = other.data_;
    other.data_ = nullptr;
  }
  return *this;
}

bool AsyncHandle::cancel() {
  if (!data_ || data_->executed.load() || data_->cancelled.load()) {
    return false;
  }

  data_->cancelled.store(true);

  // Attempt to cancel via LVGL
  // Note: lv_async_call_cancel matches by callback pointer AND user_data
  lv_result_t result = lv_async_call_cancel(async_cancellable_proxy, data_);

  return result == LV_RESULT_OK;
}

bool AsyncHandle::valid() const {
  return data_ != nullptr && !data_->executed.load() &&
         !data_->cancelled.load();
}

void AsyncHandle::release() {
  if (data_) {
    data_->owned.store(false);
    data_ = nullptr;
  }
}

// Async implementation

lv_result_t Async::call(std::function<void()> callback) {
  auto* data = new AsyncHandle::CallbackData();
  data->callback = std::move(callback);
  data->owned.store(false);  // Fire-and-forget, proxy owns data

  lv_result_t result = lv_async_call(async_fire_and_forget_proxy, data);
  if (result != LV_RESULT_OK) {
    delete data;
  }
  return result;
}

AsyncHandle Async::call_cancellable(std::function<void()> callback) {
  auto* data = new AsyncHandle::CallbackData();
  data->callback = std::move(callback);
  data->owned.store(true);  // Handle owns data

  lv_result_t result = lv_async_call(async_cancellable_proxy, data);
  if (result != LV_RESULT_OK) {
    delete data;
    return AsyncHandle();  // Return invalid handle
  }

  return AsyncHandle(data);
}

}  // namespace lvgl
