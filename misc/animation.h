#ifndef LVGL_CPP_MISC_ANIMATION_H_
#define LVGL_CPP_MISC_ANIMATION_H_

#include <cstdint>
#include <functional>
#include <memory>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

namespace lvgl {

class Animation {
 public:
  using ExecCallback = std::function<void(void*, int32_t)>;
  using CompletedCallback = std::function<void()>;

  Animation();

  /**
   * @brief Construct an animation with common parameters.
   * @param var The object/variable to animate.
   * @param start_val Start value.
   * @param end_val End value.
   * @param duration Duration in milliseconds.
   */
  Animation(void* var, int32_t start_val, int32_t end_val, uint32_t duration);

  ~Animation();

  // Builder pattern

  /**
   * @brief Set the object or variable to animate.
   * @param var Pointer to the variable/object.
   */
  Animation& set_var(void* var);

  /**
   * @brief Set a C-style execution callback.
   * @param exec_cb The callback function.
   */
  Animation& set_exec_cb(lv_anim_exec_xcb_t exec_cb);

  /**
   * @brief Set a C++ execution callback.
   * The callback is stored internally and managed by the Animation wrapper.
   * @param cb The `std::function` callback.
   */
  Animation& set_exec_cb(ExecCallback cb);

  /**
   * @brief Set a completion callback.
   * @param cb The callback to run when animation completes.
   */
  Animation& set_completed_cb(CompletedCallback cb);

  /**
   * @brief Set a deletion callback.
   * @param cb The callback to run when animation is deleted.
   */
  Animation& set_deleted_cb(std::function<void()> cb);

  /**
   * @brief Set the duration of the animation.
   * @param duration Duration in milliseconds.
   */
  Animation& set_duration(uint32_t duration);

  /**
   * @brief Set a delay before starting the animation.
   * @param delay Delay in milliseconds.
   */
  Animation& set_delay(uint32_t delay);

  /**
   * @brief Set the start and end values.
   * @param start Start value.
   * @param end End value.
   */
  Animation& set_values(int32_t start, int32_t end);

  /**
   * @brief Set the path (easing) function.
   * @param path_cb The LVGL path callback (e.g., `lv_anim_path_linear`).
   */
  Animation& set_path_cb(lv_anim_path_cb_t path_cb);

  /**
   * @brief Set the repeat count.
   * @param cnt Number of repetitions (`LV_ANIM_REPEAT_INFINITE` for infinite).
   */
  Animation& set_repeat_count(uint32_t cnt);

  /**
   * @brief Set the delay before repetition.
   * @param delay Delay in milliseconds.
   */
  Animation& set_repeat_delay(uint32_t delay);

  /**
   * @brief Set the playback duration (reverse direction).
   * @param duration Duration in milliseconds.
   */
  Animation& set_playback_duration(uint32_t duration);

  /**
   * @brief Set the playback delay.
   * @param delay Delay in milliseconds.
   */
  Animation& set_playback_delay(uint32_t delay);

  void start();

 private:
  lv_anim_t anim_;

  // Internal closure data to bridge C callbacks to C++ std::function
  struct CallbackData {
    ExecCallback exec_cb;
    CompletedCallback completed_cb;
    std::function<void()> deleted_cb;
  };

  std::unique_ptr<CallbackData> user_data_;

  static void exec_cb_proxy(lv_anim_t* a, int32_t v);
  static void completed_cb_proxy(lv_anim_t* a);
  static void deleted_cb_proxy(lv_anim_t* a);
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ANIMATION_H_
