#ifndef LVGL_CPP_WIDGETS_BUTTON_MATRIX_H_
#define LVGL_CPP_WIDGETS_BUTTON_MATRIX_H_


#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export

/**
 * @file button_matrix.h
 * @brief C++ Wrapper for LVGL ButtonMatrix Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::ButtonMatrix widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class ButtonMatrix : public Object {
public:
  /**
   * @brief Create a ButtonMatrix on the active screen.
   */
  ButtonMatrix();
  /**
   * @brief Create a ButtonMatrix with a parent.
   * @param parent The parent object.
   */
  explicit ButtonMatrix(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit ButtonMatrix(lv_obj_t* obj);

  void set_map(const char* const map[]);
  void set_ctrl_map(const lv_buttonmatrix_ctrl_t ctrl_map[]);
  void set_selected_button(uint32_t btn_id);
  void set_button_ctrl(uint32_t btn_id, lv_buttonmatrix_ctrl_t ctrl);
  void clear_button_ctrl(uint32_t btn_id, lv_buttonmatrix_ctrl_t ctrl);
  void set_button_ctrl_all(lv_buttonmatrix_ctrl_t ctrl);
  void clear_button_ctrl_all(lv_buttonmatrix_ctrl_t ctrl);
  void set_button_width(uint32_t btn_id, uint32_t width);
  void set_one_checked(bool en);

  const char* const *get_map();
  uint32_t get_selected_button();
  const char* get_button_text(uint32_t btn_id);
  bool has_button_ctrl(uint32_t btn_id, lv_buttonmatrix_ctrl_t ctrl);
  bool get_one_checked();
};

/**
 * @file button_matrix.h
 * @brief C++ Wrapper for LVGL ButtonMatrix Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::ButtonMatrix widget(lv_screen_active());
 * widget.center();
 * ```
 */
} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_BUTTON_MATRIX_H_
