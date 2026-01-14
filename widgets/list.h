#ifndef LVGL_CPP_WIDGETS_LIST_H_
#define LVGL_CPP_WIDGETS_LIST_H_

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export


#if LV_USE_LIST
/**
 * @file list.h
 * @brief C++ Wrapper for LVGL List Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::List widget(lv_screen_active());
 * widget.center();
 * lvgl::Label text_label = widget.add_text("Header");
 * lvgl::Button btn = widget.add_button(LV_SYMBOL_OK, "Apply");
 * ```
 */
namespace lvgl {

class Button;
class Label;
class List : public Object {
 public:
  /**
   * @brief Create a List on the active screen.
   */
  List();
  /**
   * @brief Create a List with a parent.
   * @param parent The parent object.
   */
  explicit List(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit List(lv_obj_t* obj);

  Label add_text(const char* txt);
  Button add_button(const void* icon, const char* txt);
  const char* get_button_text(lv_obj_t* btn);
  void set_button_text(lv_obj_t* btn, const char* txt);
};

}  // namespace lvgl


#endif // LV_USE_LIST
#endif  // LVGL_CPP_WIDGETS_LIST_H_
