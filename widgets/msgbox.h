#ifndef LVGL_CPP_WIDGETS_MSGBOX_H_
#define LVGL_CPP_WIDGETS_MSGBOX_H_

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_MSGBOX
/**
 * @file msgbox.h
 * @brief C++ Wrapper for LVGL MsgBox Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::MsgBox widget(lv_screen_active());
 * widget.center();
 * lvgl::MsgBoxContent content = widget.get_content();
 * lvgl::Label title = widget.get_title();
 * ```
 */
namespace lvgl {

class Button;
class Label;

class MsgBoxHeader : public Object {
 public:
  using Object::Object;
};

class MsgBoxFooter : public Object {
 public:
  using Object::Object;
};

class MsgBoxContent : public Object {
 public:
  using Object::Object;
};

class MsgBox : public Object {
 public:
  /**
   * @brief Create a MsgBox on the active screen.
   */
  MsgBox();
  /**
   * @brief Create a MsgBox with a parent.
   * @param parent The parent object.
   */
  explicit MsgBox(Object& parent,
                  Ownership ownership =
                      Ownership::Default);  // Creates modal if parent is NULL?
                                            // No, lv_msgbox_create creates
                                            // modal if parent is NULL.
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit MsgBox(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Label add_title(const char* title);
  Button add_header_button(const void* icon);
  Label add_text(const char* text);
  Button add_footer_button(const char* text);
  Button add_close_button();

  // Fluent API shadows
  MsgBox& set_width(int32_t width);
  MsgBox& set_height(int32_t height);
  MsgBox& set_size(int32_t width, int32_t height);
  MsgBox& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  MsgBox& add_state(lv_state_t state);
  MsgBox& remove_state(lv_state_t state);
  MsgBox& add_flag(lv_obj_flag_t flag);
  MsgBox& remove_flag(lv_obj_flag_t flag);

  MsgBoxHeader get_header();
  MsgBoxFooter get_footer();
  MsgBoxContent get_content();
  Label get_title();

  void close();
  void close_async();
};

}  // namespace lvgl

#endif  // LV_USE_MSGBOX
#endif  // LVGL_CPP_WIDGETS_MSGBOX_H_
