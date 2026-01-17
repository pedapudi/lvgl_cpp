#ifndef LVGL_CPP_WIDGETS_WIN_H_
#define LVGL_CPP_WIDGETS_WIN_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
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
  explicit Win(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Win(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Label add_title(const char* txt);
  Button add_button(const void* icon, int32_t btn_w);

  // Fluent API shadows
  Win& set_width(int32_t width);
  Win& set_height(int32_t height);
  Win& set_size(int32_t width, int32_t height);
  Win& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Win& add_state(lv_state_t state);
  Win& remove_state(lv_state_t state);
  Win& add_flag(lv_obj_flag_t flag);
  Win& remove_flag(lv_obj_flag_t flag);
  WinHeader get_header();
  WinContent get_content();
};

}  // namespace lvgl

#endif  // LV_USE_WIN
#endif  // LVGL_CPP_WIDGETS_WIN_H_
