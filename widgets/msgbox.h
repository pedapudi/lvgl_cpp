#ifndef LVGL_CPP_WIDGETS_MSGBOX_H_
#define LVGL_CPP_WIDGETS_MSGBOX_H_

#include "../core/widget.h"  // IWYU pragma: export
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

class MsgBoxHeader : public Widget<MsgBoxHeader> {
 public:
  using Widget::Widget;
};

class MsgBoxFooter : public Widget<MsgBoxFooter> {
 public:
  using Widget::Widget;
};

class MsgBoxContent : public Widget<MsgBoxContent> {
 public:
  using Widget::Widget;
};

class MsgBox : public Widget<MsgBox> {
 public:
  MsgBox();
  explicit MsgBox(Object* parent, Ownership ownership = Ownership::Default);
  explicit MsgBox(Object& parent);
  explicit MsgBox(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Label add_title(const char* title);
  Button add_header_button(const void* icon);
  Label add_text(const char* text);
  Button add_footer_button(const char* text);
  Button add_close_button();

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
