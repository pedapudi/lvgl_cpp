#ifndef LVGL_CPP_MISC_TIMER_H_
#define LVGL_CPP_MISC_TIMER_H_

#include <cstdint>

#include <functional>
#include <memory>

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

class Timer {
 public:
  using TimerCallback = std::function<void(Timer*)>;

  Timer();
  Timer(uint32_t period, TimerCallback cb);
  ~Timer();

  void set_period(uint32_t period);
  void set_cb(TimerCallback cb);
  void ready();
  void pause();
  void resume();
  void reset();
  void set_repeat_count(int32_t repeat_count);
  void set_auto_delete(bool auto_delete);
  void enable(bool en);

  lv_timer_t* raw() const { return timer_; }

 private:
  lv_timer_t* timer_;
  std::unique_ptr<TimerCallback> cb_;

  static void timer_proxy(lv_timer_t* t);
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_TIMER_H_
