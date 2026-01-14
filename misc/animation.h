#ifndef LVGL_CPP_MISC_ANIMATION_H_
#define LVGL_CPP_MISC_ANIMATION_H_

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
  Animation& set_var(void* var);
  Animation& set_exec_cb(lv_anim_exec_xcb_t exec_cb);  // C style callback
  Animation& set_exec_cb(ExecCallback cb);  // C++ style, managed lifetime
  Animation& set_completed_cb(CompletedCallback cb);
  Animation& set_deleted_cb(std::function<void()> cb);

  Animation& set_duration(uint32_t duration);
  Animation& set_delay(uint32_t delay);
  Animation& set_values(int32_t start, int32_t end);
  Animation& set_path_cb(lv_anim_path_cb_t path_cb);
  Animation& set_repeat_count(uint32_t cnt);
  Animation& set_repeat_delay(uint32_t delay);
  Animation& set_playback_duration(uint32_t duration);
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
