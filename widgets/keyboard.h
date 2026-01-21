#ifndef LVGL_CPP_WIDGETS_KEYBOARD_H_
#define LVGL_CPP_WIDGETS_KEYBOARD_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_KEYBOARD
/**
 * @file keyboard.h
 * @brief C++ Wrapper for LVGL Keyboard Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Keyboard widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Keyboard : public Widget<Keyboard> {
 public:
  Keyboard();
  explicit Keyboard(Object* parent, Ownership ownership = Ownership::Default);
  explicit Keyboard(Object& parent);
  explicit Keyboard(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Keyboard& set_textarea(lv_obj_t* ta);
  Keyboard& set_textarea(Object& ta);
  Keyboard& set_mode(lv_keyboard_mode_t mode);
  Keyboard& set_mode(KeyboardMode mode) {
    return set_mode(static_cast<lv_keyboard_mode_t>(mode));
  }
  Keyboard& set_popovers(bool en);
  Keyboard& set_map(lv_keyboard_mode_t mode, const char* const map[],
                    const lv_buttonmatrix_ctrl_t ctrl_map[]);
  Keyboard& set_map(KeyboardMode mode, const char* const map[],
                    const lv_buttonmatrix_ctrl_t ctrl_map[]) {
    return set_map(static_cast<lv_keyboard_mode_t>(mode), map, ctrl_map);
  }

  lv_obj_t* get_textarea();
  lv_keyboard_mode_t get_mode();
  bool get_popovers();
  const char* const* get_map_array();
  uint32_t get_selected_button();
  const char* get_button_text(uint32_t btn_id);
};

}  // namespace lvgl

#endif  // LV_USE_KEYBOARD
#endif  // LVGL_CPP_WIDGETS_KEYBOARD_H_
