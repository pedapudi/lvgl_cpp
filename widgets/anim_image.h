#ifndef LVGL_CPP_WIDGETS_ANIM_IMAGE_H_
#define LVGL_CPP_WIDGETS_ANIM_IMAGE_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_ANIMIMG
/**
 * @file anim_image.h
 * @brief C++ Wrapper for LVGL AnimImage Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::AnimImage widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class AnimImage : public Widget<AnimImage> {
 public:
  /**
   * @brief Create a AnimImage on the active screen.
   */
  AnimImage();
  /**
   * @brief Create a AnimImage with a parent.
   * @param parent The parent object.
   */
  explicit AnimImage(Object& parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit AnimImage(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  AnimImage& set_src(const void* dsc[], size_t num);
  AnimImage& start();
  AnimImage& set_duration(uint32_t duration);
  AnimImage& set_repeat_count(uint32_t count);
};

/**
 * @file anim_image.h
 * @brief C++ Wrapper for LVGL AnimImage Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::AnimImage widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl

#endif  // LV_USE_ANIMIMG
#endif  // LVGL_CPP_WIDGETS_ANIM_IMAGE_H_
