#ifndef LVGL_CPP_WIDGETS_ANIM_IMAGE_H_
#define LVGL_CPP_WIDGETS_ANIM_IMAGE_H_


#include "image.h"
#include "lvgl.h"

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

class AnimImage : public Image {
public:
  /**
   * @brief Create a AnimImage on the active screen.
   */
  AnimImage();
  /**
   * @brief Create a AnimImage with a parent.
   * @param parent The parent object.
   */
  explicit AnimImage(Object *parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit AnimImage(lv_obj_t *obj);

  void set_src(const void *dsc[], size_t num);
  void start();
  void set_duration(uint32_t duration);
  void set_repeat_count(uint32_t count);
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
} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_ANIM_IMAGE_H_
