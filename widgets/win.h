#ifndef LVGL_CPP_WIDGETS_WIN_H_
#define LVGL_CPP_WIDGETS_WIN_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_WIN
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

class Win : public Widget<Win> {
 public:
  /**
   * @brief Create a Win on the active screen.
   */
  Win();
  /**
   * @brief Create a Win with a parent.
   * @param parent The parent object.
   */
  explicit Win(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Win(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Label add_title(const char* txt);
  Button add_button(const void* icon, int32_t btn_w);

  WinHeader get_header();
  WinContent get_content();
};

}  // namespace lvgl

#endif  // LV_USE_WIN
#endif  // LVGL_CPP_WIDGETS_WIN_H_
