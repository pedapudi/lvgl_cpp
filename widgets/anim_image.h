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
  AnimImage();
  explicit AnimImage(Object* parent, Ownership ownership = Ownership::Default);
  explicit AnimImage(Object& parent);
  explicit AnimImage(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  AnimImage& set_src(const void* dsc[], size_t num);
  AnimImage& start();
  AnimImage& set_duration(uint32_t duration);
  AnimImage& set_repeat_count(uint32_t count);
};

}  // namespace lvgl

#endif  // LV_USE_ANIMIMG
#endif  // LVGL_CPP_WIDGETS_ANIM_IMAGE_H_
