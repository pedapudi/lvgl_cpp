#ifndef LVGL_CPP_WIDGETS_TEXTAREA_H_
#define LVGL_CPP_WIDGETS_TEXTAREA_H_

#include <cstdint>

#include "../core/widget.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_TEXTAREA
/**
 * @file textarea.h
 * @brief C++ Wrapper for LVGL Textarea Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Textarea widget(lv_screen_active());
 * widget.center();
 * lvgl::Label label_part = widget.get_label();
 * ```
 */
namespace lvgl {

class Label;
class Textarea : public Widget<Textarea> {
 public:
  using Widget::Widget;

  /**
   * @brief Create a Textarea on the active screen.
   */
  Textarea();

  /**
   * @brief Create a Textarea alias with a parent.
   * @param parent The parent object.
   */
  explicit Textarea(Object* parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a new Textarea with parent (Reference).
   * @param parent Parent object.
   */
  explicit Textarea(Object& parent);

  /**
   * @brief Create a Textarea with a parent and text.
   * @param parent The parent object.
   * @param text The initial text.
   */
  Textarea(Object& parent, const char* text);

  /**
   * @brief Create a Textarea with a parent and text.
   * @param parent The parent object.
   * @param text The initial text.
   */
  Textarea(Object& parent, const std::string& text);

  /**
   * @brief Create a Textarea with a parent and text.
   * @param parent The parent object.
   * @param text The initial text.
   */
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Textarea(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Textarea& add_char(uint32_t c);
  Textarea& add_text(const char* txt);
  Textarea& delete_char();
  Textarea& delete_char_forward();
  Textarea& set_text(const char* txt);
  Textarea& set_placeholder_text(const char* txt);
  Textarea& set_cursor_pos(int32_t pos);
  Textarea& set_cursor_click_pos(bool en);
  Textarea& set_password_mode(bool en);
  Textarea& set_password_bullet(const char* bullet);
  Textarea& set_one_line(bool en);
  Textarea& set_accepted_chars(const char* list);
  Textarea& set_max_length(uint32_t num);
  Textarea& set_insert_replace(const char* txt);
  Textarea& set_text_selection(bool en);
  Textarea& set_password_show_time(uint32_t time);
  Textarea& set_align(lv_text_align_t align);

  const char* get_text() const;
  const char* get_placeholder_text();
  Label get_label();
  uint32_t get_cursor_pos();
  bool get_cursor_click_pos();
  bool get_password_mode() const;
  const char* get_password_bullet();
  bool get_one_line() const;
  const char* get_accepted_chars();
  uint32_t get_max_length();
  bool text_is_selected();
  bool get_text_selection();
  uint32_t get_password_show_time();
  uint32_t get_current_char();

  void clear_selection();
  void cursor_right();
  void cursor_left();
  void cursor_down();
  void cursor_up();
};

}  // namespace lvgl

#endif  // LV_USE_TEXTAREA
#endif  // LVGL_CPP_WIDGETS_TEXTAREA_H_
