#ifndef LVGL_CPP_WIDGETS_IMAGE_BUTTON_H_
#define LVGL_CPP_WIDGETS_IMAGE_BUTTON_H_

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_IMAGEBUTTON
/**
 * @file image_button.h
 * @brief C++ Wrapper for LVGL ImageButton Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::ImageButton widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class ImageButton : public Object {
 public:
  /**
   * @brief Create a ImageButton on the active screen.
   */
  ImageButton();
  /**
   * @brief Create a ImageButton with a parent.
   * @param parent The parent object.
   */
  explicit ImageButton(Object* parent,
                       Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit ImageButton(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  ImageButton& set_src(lv_imagebutton_state_t state, const void* src_left,
                       const void* src_mid, const void* src_right);
  ImageButton& set_state(lv_imagebutton_state_t state);

  // Fluent API shadows
  ImageButton& set_width(int32_t width);
  ImageButton& set_height(int32_t height);
  ImageButton& set_size(int32_t width, int32_t height);
  ImageButton& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  ImageButton& add_state(lv_state_t state);
  ImageButton& remove_state(lv_state_t state);
  ImageButton& add_flag(lv_obj_flag_t flag);
  ImageButton& remove_flag(lv_obj_flag_t flag);

  const void* get_src_left(lv_imagebutton_state_t state);
  const void* get_src_middle(lv_imagebutton_state_t state);
  const void* get_src_right(lv_imagebutton_state_t state);
};

/**
 * @file image_button.h
 * @brief C++ Wrapper for LVGL ImageButton Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::ImageButton widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl

#endif  // LV_USE_IMAGEBUTTON
#endif  // LVGL_CPP_WIDGETS_IMAGE_BUTTON_H_
