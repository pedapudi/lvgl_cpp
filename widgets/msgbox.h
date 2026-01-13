#ifndef LVGL_CPP_WIDGETS_MSGBOX_H_
#define LVGL_CPP_WIDGETS_MSGBOX_H_

#include "../core/object.h"
#include "lvgl.h"

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
  explicit MsgBox(Object *parent); // Creates modal if parent is NULL?
                                   // No, lv_msgbox_create creates
                                   // modal if parent is NULL.
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit MsgBox(lv_obj_t *obj);

  Label add_title(const char *title);
  Button add_header_button(const void *icon);
  Label add_text(const char *text);
  Button add_footer_button(const char *text);
  Button add_close_button();

  MsgBoxHeader get_header();
  MsgBoxFooter get_footer();
  MsgBoxContent get_content();
  Label get_title();

  void close();
  void close_async();
};

} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_MSGBOX_H_
