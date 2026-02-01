#ifndef LVGL_CPP_WIDGETS_BUTTON_MATRIX_H_
#define LVGL_CPP_WIDGETS_BUTTON_MATRIX_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_BUTTONMATRIX
/**
 * @file button_matrix.h
 * @brief User Guide:
 * `ButtonMatrix` (also known as `Btnm`) can display multiple buttons in a grid.
 * It is very memory-efficient as it creates only one `lv_obj_t` for the entire
 * matrix.
 *
 * Key Features:
 * - **Maps**: Define button text and layout using a simple array of strings.
 * - **Control Flags**: Set hidden, checkable, or disabled states per-button.
 * - **Styling**: Can style the background and the individual buttons (items).
 *
 * Example:
 * `const char* map[] = {"Btn1", "Btn2", "\n", "Btn3", ""};`
 * `ButtonMatrix(parent, map).center();`
 */
namespace lvgl {

class ButtonMatrix : public Widget<ButtonMatrix> {
 public:
  /**
   * @brief Button control flags.
   */
  enum class Control : uint16_t {
    None = LV_BUTTONMATRIX_CTRL_NONE,
    Hidden = LV_BUTTONMATRIX_CTRL_HIDDEN,
    NoRepeat = LV_BUTTONMATRIX_CTRL_NO_REPEAT,
    Disabled = LV_BUTTONMATRIX_CTRL_DISABLED,
    Checkable = LV_BUTTONMATRIX_CTRL_CHECKABLE,
    Checked = LV_BUTTONMATRIX_CTRL_CHECKED,
    ClickTrig = LV_BUTTONMATRIX_CTRL_CLICK_TRIG,
    Popover = LV_BUTTONMATRIX_CTRL_POPOVER,
    Recolor = LV_BUTTONMATRIX_CTRL_RECOLOR,
    Custom1 = LV_BUTTONMATRIX_CTRL_CUSTOM_1,
    Custom2 = LV_BUTTONMATRIX_CTRL_CUSTOM_2,
  };
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
  ButtonMatrix& set_button_ctrl(uint32_t btn_id, Control ctrl);

  ButtonMatrix& clear_button_ctrl(uint32_t btn_id, Control ctrl);

  ButtonMatrix& set_button_ctrl_all(Control ctrl);

  ButtonMatrix& clear_button_ctrl_all(Control ctrl);
  ButtonMatrix& set_button_width(uint32_t btn_id, uint32_t width);
  ButtonMatrix& set_one_checked(bool en);

  const char* const* get_map();
  uint32_t get_selected_button();
  const char* get_button_text(uint32_t btn_id);
  bool has_button_ctrl(uint32_t btn_id, Control ctrl);
  bool get_one_checked();
};

}  // namespace lvgl

#endif  // LV_USE_BUTTONMATRIX
#endif  // LVGL_CPP_WIDGETS_BUTTON_MATRIX_H_
