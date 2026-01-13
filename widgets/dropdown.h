#ifndef LVGL_CPP_WIDGETS_DROPDOWN_H_
#define LVGL_CPP_WIDGETS_DROPDOWN_H_

#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export

/**
 * @file dropdown.h
 * @brief C++ Wrapper for LVGL Dropdown Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Dropdown widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {
class Subject;
class Observer;

class Dropdown : public Object {
public:
  /**
   * @brief Create a Dropdown on the active screen.
   */
  Dropdown();
  /**
   * @brief Create a Dropdown with a parent.
   * @param parent The parent object.
   */
  explicit Dropdown(Object *parent);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Dropdown(lv_obj_t *obj);

  void set_text(const char *txt);
  void set_options(const char *options);
  void set_options_static(const char *options);
  void add_option(const char *option, uint32_t pos);
  void clear_options();
  void set_selected(uint32_t sel_opt);
  void set_dir(lv_dir_t dir);
  void set_symbol(const void *symbol);
  void set_selected_highlight(bool en);

  lv_obj_t *get_list();
  const char *get_text();
  const char *get_options();
  uint32_t get_selected();
  uint32_t get_option_count();
  void get_selected_str(char *buf, uint32_t buf_size);
  int32_t get_option_index(const char *option);
  const char *get_symbol();
  bool get_selected_highlight();
  lv_dir_t get_dir();

  void open();
  void close();
  bool is_open();

  /**
   * @brief Bind the dropdown's selected option to an integer subject.
   * @param subject The integer subject to bind.
   * @return An Observer object. Keep this object alive to maintain the binding.
   */
  [[nodiscard]] Observer bind_value(Subject &subject);
};

/**
 * @file dropdown.h
 * @brief C++ Wrapper for LVGL Dropdown Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Dropdown widget(lv_screen_active());
 * widget.center();
 * ```
 */
} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_DROPDOWN_H_
