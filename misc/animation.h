#ifndef LVGL_CPP_MISC_ANIMATION_H_
#define LVGL_CPP_MISC_ANIMATION_H_

#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export
#include <functional>
#include <memory>

namespace lvgl {

class Animation {
public:
  using ExecCallback = std::function<void(void *, int32_t)>;
  using CompletedCallback = std::function<void()>;

  Animation();
  ~Animation();

  // Builder pattern
  Animation &set_var(void *var);
  Animation &set_exec_cb(lv_anim_exec_xcb_t exec_cb); // C style callback
  Animation& set_exec_cb(ExecCallback cb); // C++ style, requires keeping Animation object
                                // alive or careful management
  Animation &set_duration(uint32_t duration);
  Animation &set_delay(uint32_t delay);
  Animation &set_values(int32_t start, int32_t end);
  Animation &set_path_cb(lv_anim_path_cb_t path_cb);
  Animation &set_repeat_count(uint32_t cnt);
  Animation &set_repeat_delay(uint32_t delay);
  Animation &set_playback_duration(uint32_t duration);
  Animation &set_playback_delay(uint32_t delay);

  void start();

private:
  lv_anim_t anim_;
  std::unique_ptr<ExecCallback> exec_cb_cpp_;

  static void exec_cb_proxy(void *var, int32_t v);
};

} // namespace lvgl

#endif // LVGL_CPP_MISC_ANIMATION_H_
