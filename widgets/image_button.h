#ifndef LVGL_CPP_WIDGETS_IMAGE_BUTTON_H_
#define LVGL_CPP_WIDGETS_IMAGE_BUTTON_H_

#include "../core/widget.h"  // IWYU pragma: export
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

class ImageButton : public Widget<ImageButton> {
 public:
  ImageButton();
  explicit ImageButton(Object* parent,
                       Ownership ownership = Ownership::Default);
  explicit ImageButton(Object& parent);
  explicit ImageButton(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief ImageButton states.
   */
  enum class State : uint8_t {
    Released = LV_IMAGEBUTTON_STATE_RELEASED,
    Pressed = LV_IMAGEBUTTON_STATE_PRESSED,
    Disabled = LV_IMAGEBUTTON_STATE_DISABLED,
    CheckedReleased = LV_IMAGEBUTTON_STATE_CHECKED_RELEASED,
    CheckedPressed = LV_IMAGEBUTTON_STATE_CHECKED_PRESSED,
    CheckedDisabled = LV_IMAGEBUTTON_STATE_CHECKED_DISABLED,
  };

  ImageButton& set_src(State state, const void* src_left, const void* src_mid,
                       const void* src_right);
  ImageButton& set_state(State state);

  const void* get_src_left(State state);
  const void* get_src_middle(State state);
  const void* get_src_right(State state);
};

}  // namespace lvgl

#endif  // LV_USE_IMAGEBUTTON
#endif  // LVGL_CPP_WIDGETS_IMAGE_BUTTON_H_
