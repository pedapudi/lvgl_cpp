#ifndef LVGL_CPP_WIDGETS_IMAGE_H_
#define LVGL_CPP_WIDGETS_IMAGE_H_

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export


#if LV_USE_IMAGE
/**
 * @file image.h
 * @brief C++ Wrapper for LVGL Image Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Image widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Image : public Object {
 public:
  /**
   * @brief Create a Image on the active screen.
   */
  Image();
  /**
   * @brief Create a Image with a parent.
   * @param parent The parent object.
   */
  explicit Image(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Image(lv_obj_t* obj);

  void set_src(const void* src);
  void set_offset_x(int32_t x);
  void set_offset_y(int32_t y);
  void set_rotation(int32_t angle);
  void set_pivot(int32_t x, int32_t y);
  void set_scale(uint32_t zoom);
  void set_scale_x(uint32_t zoom);
  void set_scale_y(uint32_t zoom);
  void set_blend_mode(lv_blend_mode_t blend_mode);
  void set_antialias(bool antialias);
  void set_inner_align(lv_image_align_t align);

  const void* get_src() const;
  int32_t get_offset_x();
  int32_t get_offset_y();
  int32_t get_rotation();
  void get_pivot(lv_point_t* pivot);
  int32_t get_scale();
  int32_t get_scale_x();
  int32_t get_scale_y();
  int32_t get_src_width();
  int32_t get_src_height();
  lv_blend_mode_t get_blend_mode();
  bool get_antialias();
  lv_image_align_t get_inner_align();
};

/**
 * @file image.h
 * @brief C++ Wrapper for LVGL Image Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Image widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl


#endif // LV_USE_IMAGE
#endif  // LVGL_CPP_WIDGETS_IMAGE_H_
