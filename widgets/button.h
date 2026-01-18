#ifndef LVGL_CPP_WIDGETS_BUTTON_H_
#define LVGL_CPP_WIDGETS_BUTTON_H_

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_BUTTON
/**
 * @file button.h
 * @brief C++ Wrapper for LVGL Button Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Button btn(lv_screen_active());
 * btn.set_size(120, 50);
 * btn.center();
 *
 * btn.add_event_cb(LV_EVENT_CLICKED, [](lv_event_t* e) {
 *     printf("Clicked!\n");
 * });
 * ```
 */

namespace lvgl {

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
