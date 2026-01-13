#ifndef LVGL_CPP_WIDGETS_KEYBOARD_H_
#define LVGL_CPP_WIDGETS_KEYBOARD_H_

#include "button_matrix.h"  // IWYU pragma: export
#include "lvgl.h"           // IWYU pragma: export

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
  explicit Keyboard(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Keyboard(lv_obj_t* obj);

  void set_textarea(lv_obj_t* ta);
  void set_textarea(Object* ta);
  void set_mode(lv_keyboard_mode_t mode);
  void set_popovers(bool en);
  void set_map(lv_keyboard_mode_t mode, const char* const map[],
               const lv_buttonmatrix_ctrl_t ctrl_map[]);

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

#endif  // LVGL_CPP_WIDGETS_KEYBOARD_H_
