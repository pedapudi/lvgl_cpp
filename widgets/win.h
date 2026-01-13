#ifndef LVGL_CPP_WIDGETS_WIN_H_
#define LVGL_CPP_WIDGETS_WIN_H_

#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export

/**
 * @file win.h
 * @brief C++ Wrapper for LVGL Win Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Win widget(lv_screen_active());
 * widget.center();
 * lvgl::WinContent content = widget.get_content();
 * lvgl::Label label(&content);
 * ```
 */
namespace lvgl {

class Button;
class Label;

class WinHeader : public Object {
public:
  using Object::Object;
};

class WinContent : public Object {
public:
  using Object::Object;
};

class Win : public Object {
public:
  /**
   * @brief Create a Win on the active screen.
   */
  Win();
  /**
   * @brief Create a Win with a parent.
   * @param parent The parent object.
   */
  explicit Win(Object* parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Win(lv_obj_t* obj);

  Label add_title(const char* txt);
  Button add_button(const void* icon, int32_t btn_w);
  WinHeader get_header();
  WinContent get_content();
};

} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_WIN_H_
