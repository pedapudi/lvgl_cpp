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

  /**
   * @brief Set the timer period.
   * @param period Period in milliseconds.
   */
  void set_period(uint32_t period);

  /**
   * @brief Set the timer callback.
   * @param cb The callback function.
   */
  void set_cb(TimerCallback cb);

  /**
   * @brief Mark the timer as ready to run immediately.
   */
  void ready();

  /**
   * @brief Pause the timer.
   */
  void pause();

  /**
   * @brief Resume the timer.
   */
  void resume();

  /**
   * @brief Reset the timer (start counting from 0).
   */
  void reset();

  /**
   * @brief Set the number of repetitions.
   * @param repeat_count Number of repeats (-1 for infinite).
   */
  void set_repeat_count(int32_t repeat_count);

  /**
   * @brief Enable auto-deletion of the timer after it runs out.
   * @param auto_delete true to auto-delete.
   */
  void set_auto_delete(bool auto_delete);

  /**
   * @brief Enable or disable the timer.
   * @param en true to enable.
   */
  void enable(bool en);

  lv_timer_t* raw() const { return timer_; }

 private:
  lv_timer_t* timer_;
  std::unique_ptr<TimerCallback> cb_;

  static void timer_proxy(lv_timer_t* t);
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_TIMER_H_
