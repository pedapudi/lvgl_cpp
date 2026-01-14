#ifndef LVGL_CPP_MISC_ANIM_EXEC_CALLBACK_H_
#define LVGL_CPP_MISC_ANIM_EXEC_CALLBACK_H_

#include <cstdint>
#include <memory>

namespace lvgl {

class AnimExecCallback {
 public:
  virtual ~AnimExecCallback() = default;

  /**
   * @brief Execute the animation step.
   * @param var The variable/object being animated.
   * @param val The current animation value.
   */
  virtual void operator()(void* var, int32_t val) = 0;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ANIM_EXEC_CALLBACK_H_
