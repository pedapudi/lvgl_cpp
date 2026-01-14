#ifndef LVGL_CPP_MISC_ANIM_PATH_CALLBACK_H_
#define LVGL_CPP_MISC_ANIM_PATH_CALLBACK_H_

#include <cstdint>
#include <memory>

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

/**
 * @brief Abstract base class for animation path (easing) callbacks.
 * Implement the operator() to define custom easing curves.
 */
class AnimPathCallback {
 public:
  virtual ~AnimPathCallback() = default;

  /**
   * @brief Calculate the path value.
   * @param a Pointer to the animation descriptor.
   * @return The calculated value.
   */
  virtual int32_t operator()(const lv_anim_t* a) = 0;
};

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_ANIM_PATH_CALLBACK_H_
