#ifndef LVGL_CPP_WIDGETS_BUTTON_H_
#define LVGL_CPP_WIDGETS_BUTTON_H_

#include "../core/object.h"  // IWYU pragma: export
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
class Button : public Object {
 public:
  /**
   * @brief Create a Button on the active screen.
   * Takes ownership.
   */
  Button();

  /**
   * @brief Create a Button with a parent.
   * @param parent The parent object.
   * @param ownership Ownership policy.
   */
  explicit Button(Object& parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Wrap an existing lv_button object.
   * @param obj The raw LVGL object to wrap.
   * @param ownership Ownership policy.
   */
  explicit Button(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  // Fluent API shadows
  Button& set_width(int32_t width);
  Button& set_height(int32_t height);
  Button& set_size(int32_t width, int32_t height);
  Button& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
};

}  // namespace lvgl

#endif  // LV_USE_BUTTON
#endif  // LVGL_CPP_WIDGETS_BUTTON_H_
