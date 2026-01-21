#include "timer.h"

namespace lvgl {

void Timer::timer_proxy(lv_timer_t* t) {
  auto* timer_instance = static_cast<Timer*>(lv_timer_get_user_data(t));
  if (timer_instance && timer_instance->cb_ && *timer_instance->cb_) {
    (*timer_instance->cb_)(timer_instance);
  }
}

Timer::Timer() : timer_(nullptr) {}

Timer::Timer(uint32_t period, TimerCallback cb) {
  cb_ = std::make_unique<TimerCallback>(cb);
  timer_ = lv_timer_create(timer_proxy, period, this);
}

Timer::~Timer() {
  if (timer_) {
    lv_timer_delete(timer_);
    timer_ = nullptr;
  }
}

Timer::Timer(Timer&& other) noexcept
    : timer_(other.timer_), cb_(std::move(other.cb_)) {
  other.timer_ = nullptr;
  if (timer_) {
    lv_timer_set_user_data(timer_, this);
  }
}

Timer& Timer::operator=(Timer&& other) noexcept {
  if (this != &other) {
    if (timer_) {
      lv_timer_delete(timer_);
    }
    timer_ = other.timer_;
    cb_ = std::move(other.cb_);
    other.timer_ = nullptr;
    if (timer_) {
      lv_timer_set_user_data(timer_, this);
    }
  }
  return *this;
}

namespace {
struct OneshotData {
  std::function<void()> cb;
};

void oneshot_proxy(lv_timer_t* t) {
  auto* data = static_cast<OneshotData*>(lv_timer_get_user_data(t));
  if (data && data->cb) {
    data->cb();
  }
  // Data is deleted by lv_timer auto_delete mechanism?
  // No, lv_timer doesn't delete user_data automatically.
  // We need to delete it here since the timer is about to be auto-deleted.
  delete data;
}
}  // namespace

void Timer::oneshot(uint32_t delay, std::function<void()> cb) {
  auto* data = new OneshotData{std::move(cb)};
  lv_timer_t* t = lv_timer_create(oneshot_proxy, delay, data);
  lv_timer_set_repeat_count(t, 1);
  lv_timer_set_auto_delete(t, true);
}

Timer Timer::periodic(uint32_t period, TimerCallback cb) {
  return Timer(period, cb);
}

void Timer::set_period(uint32_t period) {
  if (timer_) lv_timer_set_period(timer_, period);
}

void Timer::set_cb(TimerCallback cb) {
  if (!cb_) {
    cb_ = std::make_unique<TimerCallback>(cb);
  } else {
    *cb_ = cb;
  }
  // Ensure user data points to this instance if we updated callback
  if (timer_) {
    lv_timer_set_user_data(timer_, this);
  }
}

void Timer::enable(bool en) { lv_timer_enable(en); }

std::function<void()> Timer::resume_handler_ = nullptr;

void Timer::set_resume_handler(std::function<void()> callback) {
  resume_handler_ = std::move(callback);
  if (resume_handler_) {
    lv_timer_handler_set_resume_cb(resume_handler_proxy, nullptr);
  } else {
    lv_timer_handler_set_resume_cb(nullptr, nullptr);
  }
}

void Timer::clear_resume_handler() {
  resume_handler_ = nullptr;
  lv_timer_handler_set_resume_cb(nullptr, nullptr);
}

void Timer::resume_handler_proxy(void* data) {
  if (resume_handler_) {
    resume_handler_();
  }
}

}  // namespace lvgl
