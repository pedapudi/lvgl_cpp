#include "timer.h"

namespace lvgl {

void Timer::timer_proxy(lv_timer_t *t) {
  auto *timer_instance = static_cast<Timer *>(lv_timer_get_user_data(t));
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
  }
}

void Timer::set_period(uint32_t period) {
  if (timer_)
    lv_timer_set_period(timer_, period);
}

void Timer::set_cb(TimerCallback cb) {
  if (!cb_) {
    cb_ = std::make_unique<TimerCallback>(cb);
  } else {
    *cb_ = cb;
  }
  // If timer was empty, we might need to recreate or set user data if wrapped
  // differently For now assume created with constructor
}

void Timer::ready() {
  if (timer_)
    lv_timer_ready(timer_);
}

void Timer::pause() {
  if (timer_)
    lv_timer_pause(timer_);
}

void Timer::resume() {
  if (timer_)
    lv_timer_resume(timer_);
}

void Timer::reset() {
  if (timer_)
    lv_timer_reset(timer_);
}

void Timer::set_repeat_count(int32_t repeat_count) {
  if (timer_)
    lv_timer_set_repeat_count(timer_, repeat_count);
}

void Timer::set_auto_delete(bool auto_delete) {
  if (timer_)
    lv_timer_set_auto_delete(timer_, auto_delete);
}

void Timer::enable(bool en) { lv_timer_enable(en); }

} // namespace lvgl
