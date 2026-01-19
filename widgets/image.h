#ifndef LVGL_CPP_WIDGETS_IMAGE_H_
#define LVGL_CPP_WIDGETS_IMAGE_H_

#include <cstdint>
#include <string>

#include "../core/widget.h"  // IWYU pragma: export
#include "../draw/image_descriptor.h"
#include "lvgl.h"  // IWYU pragma: export

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

class Image : public Widget<Image> {
 public:
  Image();
  explicit Image(Object* parent, Ownership ownership = Ownership::Default);
  explicit Image(Object& parent);

  /**
   * @brief Create a Image on the active screen with source.
   * @param parent The parent object.
   * @param src The image source (path or pointer).
   */
  Image(Object& parent, const void* src);

  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Image(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Set the image source.
   * @param src Pointer to image descriptor or path.
   * @note If src is a path, it must remain valid (static or global).
   * Use set_src(const std::string&) for safe path handling.
   */
  Image& set_src(const void* src);

  /**
   * @brief Set the image source from a string path.
   * @param path The file path. The string content is copied by LVGL.
   */
  Image& set_src(const std::string& path);

  /**
   * @brief Set the image source from an ImageDescriptor.
   * @param dsc The image descriptor.
   */
  Image& set_src(const ImageDescriptor& dsc);

  Image& set_offset_x(int32_t x);
  Image& set_offset_y(int32_t y);
  Image& set_rotation(int32_t angle);
  Image& set_pivot(int32_t x, int32_t y);
  Image& set_scale(uint32_t zoom);
  Image& set_scale_x(uint32_t zoom);
  Image& set_scale_y(uint32_t zoom);
  Image& set_blend_mode(lv_blend_mode_t blend_mode);
  Image& set_antialias(bool antialias);
  Image& set_inner_align(lv_image_align_t align);

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

#endif  // LV_USE_IMAGE
#endif  // LVGL_CPP_WIDGETS_IMAGE_H_
