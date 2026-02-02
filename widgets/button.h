#ifndef LVGL_CPP_WIDGETS_BUTTON_H_
#define LVGL_CPP_WIDGETS_BUTTON_H_

#include "../core/traits.h"
#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_BUTTON
/**
 * @file button.h
 * @brief User Guide:
 * The `Button` widget is a simple clickable container. It is often used with
 * a `Label` child to create buttons with text.
 *
 * Key Features:
 * - **Fluent API**: Inherits all `Widget` and `Object` properties.
 * - **Events**: Specifically designed for click, press, and release events.
 *
 * Example:
 * `Button(parent).set_size(100, 50).center().on_click([](auto& e){...});`
 */

namespace lvgl {
class Button;
template <>
struct class_traits<Button> {
  static const lv_obj_class_t* get() { return &lv_button_class; }
};

/**
 * @brief Wrapper for lv_button.
 * A button is a simple container that can be clicked.
 */
class Button : public Widget<Button> {
 public:
  using Widget::Widget;

  /**
   * @brief Create a new Button on the active screen.
   * Takes ownership.
   */
  Button();

  explicit Button(Object* parent, Ownership ownership = Ownership::Default);
  explicit Button(Object& parent);
  explicit Button(lv_obj_t* obj, Ownership ownership = Ownership::Default);
};

}  // namespace lvgl

#endif  // LV_USE_BUTTON
#endif  // LVGL_CPP_WIDGETS_BUTTON_H_
