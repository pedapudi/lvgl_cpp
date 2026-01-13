#ifndef LVGL_CPP_WIDGETS_TEXTAREA_H_
#define LVGL_CPP_WIDGETS_TEXTAREA_H_

#include "../core/object.h"
#include "lvgl.h"

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
class Textarea : public Object {
public:
  /**
   * @brief Create a Textarea on the active screen.
   */
  Textarea();
  /**
   * @brief Create a Textarea with a parent.
   * @param parent The parent object.
   */
  explicit Textarea(Object *parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Textarea(lv_obj_t *obj);

  void add_char(uint32_t c);
  void add_text(const char *txt);
  void delete_char();
  void delete_char_forward();
  void set_text(const char *txt);
  void set_placeholder_text(const char *txt);
  void set_cursor_pos(int32_t pos);
  void set_cursor_click_pos(bool en);
  void set_password_mode(bool en);
  void set_password_bullet(const char *bullet);
  void set_one_line(bool en);
  void set_accepted_chars(const char *list);
  void set_max_length(uint32_t num);
  void set_insert_replace(const char *txt);
  void set_text_selection(bool en);
  void set_password_show_time(uint32_t time);
  void set_align(lv_text_align_t align);

  const char *get_text() const;
  const char *get_placeholder_text();
  Label get_label();
  uint32_t get_cursor_pos();
  bool get_cursor_click_pos();
  bool get_password_mode() const;
  const char *get_password_bullet();
  bool get_one_line() const;
  const char *get_accepted_chars();
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

} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_TEXTAREA_H_
