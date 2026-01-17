#ifndef LVGL_CPP_WIDGETS_KEYBOARD_H_
#define LVGL_CPP_WIDGETS_KEYBOARD_H_

#include <cstdint>

#include "button_matrix.h"  // IWYU pragma: export
#include "lvgl.h"           // IWYU pragma: export

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

class Keyboard : public ButtonMatrix {
 public:
  /**
   * @brief Create a Keyboard on the active screen.
   */
  Keyboard();
  /**
   * @brief Create a Keyboard with a parent.
   * @param parent The parent object.
   */
  explicit Keyboard(Object* parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Keyboard(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Keyboard& set_textarea(lv_obj_t* ta);
  Keyboard& set_textarea(Object* ta);
  Keyboard& set_mode(lv_keyboard_mode_t mode);
  Keyboard& set_popovers(bool en);
  Keyboard& set_map(lv_keyboard_mode_t mode, const char* const map[],
                    const lv_buttonmatrix_ctrl_t ctrl_map[]);

  // Fluent API shadows
  Keyboard& set_width(int32_t width);
  Keyboard& set_height(int32_t height);
  Keyboard& set_size(int32_t width, int32_t height);
  Keyboard& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Keyboard& add_state(lv_state_t state);
  Keyboard& remove_state(lv_state_t state);
  Keyboard& add_flag(lv_obj_flag_t flag);
  Keyboard& remove_flag(lv_obj_flag_t flag);

  lv_obj_t* get_textarea();
  lv_keyboard_mode_t get_mode();
  bool get_popovers();
  const char* const* get_map_array();
  uint32_t get_selected_button();
  const char* get_button_text(uint32_t btn_id);
};

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
}  // namespace lvgl

#endif  // LV_USE_KEYBOARD
#endif  // LVGL_CPP_WIDGETS_KEYBOARD_H_
