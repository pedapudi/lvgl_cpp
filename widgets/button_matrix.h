#ifndef LVGL_CPP_WIDGETS_BUTTON_MATRIX_H_
#define LVGL_CPP_WIDGETS_BUTTON_MATRIX_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_BUTTONMATRIX
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

class ButtonMatrix : public Widget<ButtonMatrix> {
 public:
  ButtonMatrix();
  explicit ButtonMatrix(Object* parent,
                        Ownership ownership = Ownership::Default);
  explicit ButtonMatrix(Object& parent);
  explicit ButtonMatrix(lv_obj_t* obj,
                        Ownership ownership = Ownership::Default);

  /**
   * @brief Create a ButtonMatrix with specific map.
   * @param parent The parent object.
   * @param map The map of buttons.
   */
  ButtonMatrix(Object& parent, const char* const map[]);

  ButtonMatrix& set_map(const char* const map[]);
  ButtonMatrix& set_ctrl_map(const lv_buttonmatrix_ctrl_t ctrl_map[]);
  ButtonMatrix& set_selected_button(uint32_t btn_id);
  ButtonMatrix& set_button_ctrl(uint32_t btn_id, lv_buttonmatrix_ctrl_t ctrl);
  ButtonMatrix& clear_button_ctrl(uint32_t btn_id, lv_buttonmatrix_ctrl_t ctrl);
  ButtonMatrix& set_button_ctrl_all(lv_buttonmatrix_ctrl_t ctrl);
  ButtonMatrix& clear_button_ctrl_all(lv_buttonmatrix_ctrl_t ctrl);
  ButtonMatrix& set_button_width(uint32_t btn_id, uint32_t width);
  ButtonMatrix& set_one_checked(bool en);

  const char* const* get_map();
  uint32_t get_selected_button();
  const char* get_button_text(uint32_t btn_id);
  bool has_button_ctrl(uint32_t btn_id, lv_buttonmatrix_ctrl_t ctrl);
  bool get_one_checked();
};

}  // namespace lvgl

#endif  // LV_USE_BUTTONMATRIX
#endif  // LVGL_CPP_WIDGETS_BUTTON_MATRIX_H_
