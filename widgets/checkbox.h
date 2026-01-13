#ifndef LVGL_CPP_WIDGETS_CHECKBOX_H_
#define LVGL_CPP_WIDGETS_CHECKBOX_H_


#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export

/**
 * @file checkbox.h
 * @brief C++ Wrapper for LVGL Checkbox Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Checkbox widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {

class Checkbox : public Object {
public:
  /**
   * @brief Create a Checkbox on the active screen.
   */
  Checkbox();
  /**
   * @brief Create a Checkbox with a parent.
   * @param parent The parent object.
   */
  explicit Checkbox(Object *parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Checkbox(lv_obj_t *obj);

  void set_text(const char *txt);
  void set_text_static(const char *txt);
  const char *get_text() const;
};

/**
 * @file checkbox.h
 * @brief C++ Wrapper for LVGL Checkbox Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Checkbox widget(lv_screen_active());
 * widget.center();
 * ```
 */
} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_CHECKBOX_H_
