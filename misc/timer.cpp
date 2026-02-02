#include "timer.h"

namespace lvgl {

struct Timer::Data {
  TimerCallback cb;
  Timer* owner = nullptr;
};

void Timer::timer_proxy(lv_timer_t* t) {
  auto* data = static_cast<Data*>(lv_timer_get_user_data(t));
  if (data && data->cb) {
    if (data->owner) {
      data->cb(data->owner);
    } else {
      // Detached: Pass nullptr. Callback must handle it.
      data->cb(nullptr);
    }
  }
}

Timer::Timer() : timer_(nullptr) {}

Timer::Timer(uint32_t period, TimerCallback cb) {
  data_ = new Data{std::move(cb), this};
  timer_ = lv_timer_create(timer_proxy, period, data_);
}

Timer::~Timer() {
  if (timer_) {
    lv_timer_delete(timer_);
    timer_ = nullptr;
    delete data_;
    data_ = nullptr;
  }
}

Timer::Timer(Timer&& other) noexcept
    : timer_(other.timer_), data_(other.data_) {
  other.timer_ = nullptr;
  other.data_ = nullptr;
  if (data_) {
    data_->owner = this;
    if (timer_) lv_timer_set_user_data(timer_, data_);
  }
}

Timer& Timer::operator=(Timer&& other) noexcept {
  if (this != &other) {
    if (timer_) {
      lv_timer_delete(timer_);
      delete data_;
    }
    timer_ = other.timer_;
    data_ = other.data_;
    other.timer_ = nullptr;
    other.data_ = nullptr;
    if (data_) {
      data_->owner = this;
      if (timer_) lv_timer_set_user_data(timer_, data_);
    }
  }
  return *this;
}

lv_timer_t* Timer::detach() {
  if (timer_) {
    lv_timer_t* t = timer_;
    if (data_) {
      data_->owner = nullptr;
    }
    timer_ = nullptr;
    data_ = nullptr;
    return t;
  }
  return nullptr;
}

void Timer::delete_detached(lv_timer_t* t) {
  if (t) {
    auto* data = static_cast<Data*>(lv_timer_get_user_data(t));
    delete data;
    lv_timer_delete(t);
  }
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

uint32_t Timer::handler() { return lv_timer_handler(); }

Timer& Timer::set_period(uint32_t period) {
  if (timer_) lv_timer_set_period(timer_, period);
  return *this;
}

Timer& Timer::set_cb(TimerCallback cb) {
  if (!data_) {
    data_ = new Data{std::move(cb), this};
  } else {
    data_->cb = std::move(cb);
  }
  if (timer_) {
    lv_timer_set_user_data(timer_, data_);
  }
  return *this;
}

Timer& Timer::pause() {
  if (timer_) lv_timer_pause(timer_);
  return *this;
}

Timer& Timer::resume() {
  if (timer_) lv_timer_resume(timer_);
  return *this;
}

Timer& Timer::ready() {
  if (timer_) lv_timer_ready(timer_);
  return *this;
}

Timer& Timer::reset() {
  if (timer_) lv_timer_reset(timer_);
  return *this;
}

Timer& Timer::set_repeat_count(int32_t repeat_count) {
  if (timer_) lv_timer_set_repeat_count(timer_, repeat_count);
  return *this;
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
