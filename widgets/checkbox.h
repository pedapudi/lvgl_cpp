#ifndef LVGL_CPP_WIDGETS_CHECKBOX_H_
#define LVGL_CPP_WIDGETS_CHECKBOX_H_

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_CHECKBOX
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

class Checkbox : public Widget<Checkbox> {
 public:
  using Widget::Widget;

  /**
   * @brief Create a Checkbox on the active screen.
   */
  Checkbox();

  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Checkbox(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a Checkbox with a parent.
   * @param parent The parent object.
   */
  explicit Checkbox(Object* parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a new Checkbox with parent (Reference).
   * @param parent Parent object.
   */
  explicit Checkbox(Object& parent);

  /**
   * @brief Create a Checkbox with a parent and text.
   * @param parent The parent object.
   * @param text The initial text.
   */
  Checkbox(Object& parent, const char* text);

  /**
   * @brief Create a Checkbox with a parent and text.
   * @param parent The parent object.
   * @param text The initial text.
   */
  Checkbox(Object& parent, const std::string& text);

  /**
   * @brief Set the text of the checkbox.
   * @param txt The text.
   */
  Checkbox& set_text(const char* txt);

  /**
   * @brief Set the text with a static string (no copy).
   * @param txt The text pointer.
   */
  Checkbox& set_text_static(const char* txt);

  /**
   * @brief Get the text.
   * @return The text.
   */
  const char* get_text() const;
};

}  // namespace lvgl

#endif  // LV_USE_CHECKBOX
#endif  // LVGL_CPP_WIDGETS_CHECKBOX_H_
