#ifndef LVGL_CPP_WIDGETS_CHECKBOX_H_
#define LVGL_CPP_WIDGETS_CHECKBOX_H_

#include "../core/object.h"  // IWYU pragma: export
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
  explicit Checkbox(Object* parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a Checkbox with a parent and text.
   * @param parent The parent object.
   * @param text The initial text.
   */
  Checkbox(Object* parent, const char* text);

  /**
   * @brief Create a Checkbox with a parent and text.
   * @param parent The parent object.
   * @param text The initial text.
   */
  Checkbox(Object* parent, const std::string& text);

  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Checkbox(lv_obj_t* obj, Ownership ownership = Ownership::Default);

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

  // Fluent API shadows
  Checkbox& set_width(int32_t width);
  Checkbox& set_height(int32_t height);
  Checkbox& set_size(int32_t width, int32_t height);
  Checkbox& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Checkbox& add_state(lv_state_t state);
  Checkbox& remove_state(lv_state_t state);
  Checkbox& add_flag(lv_obj_flag_t flag);
  Checkbox& remove_flag(lv_obj_flag_t flag);
};

}  // namespace lvgl

#endif  // LV_USE_CHECKBOX
#endif  // LVGL_CPP_WIDGETS_CHECKBOX_H_
