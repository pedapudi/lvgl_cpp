#ifndef LVGL_CPP_WIDGETS_DROPDOWN_H_
#define LVGL_CPP_WIDGETS_DROPDOWN_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_DROPDOWN
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
  explicit Dropdown(Object* parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Dropdown(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Dropdown& set_text(const char* txt);
  Dropdown& set_options(const char* options);
  Dropdown& set_options_static(const char* options);
  Dropdown& add_option(const char* option, uint32_t pos);
  Dropdown& clear_options();
  Dropdown& set_selected(uint32_t sel_opt);
  Dropdown& set_dir(lv_dir_t dir);
  Dropdown& set_symbol(const void* symbol);
  Dropdown& set_selected_highlight(bool en);

  // Fluent API shadows
  Dropdown& set_width(int32_t width);
  Dropdown& set_height(int32_t height);
  Dropdown& set_size(int32_t width, int32_t height);
  Dropdown& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Dropdown& add_state(lv_state_t state);
  Dropdown& remove_state(lv_state_t state);
  Dropdown& add_flag(lv_obj_flag_t flag);
  Dropdown& remove_flag(lv_obj_flag_t flag);

  lv_obj_t* get_list();
  const char* get_text();
  const char* get_options();
  uint32_t get_selected();
  uint32_t get_option_count();
  void get_selected_str(char* buf, uint32_t buf_size);
  int32_t get_option_index(const char* option);
  const char* get_symbol();
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
  [[nodiscard]] Observer bind_value(Subject& subject);
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
}  // namespace lvgl

#endif  // LV_USE_DROPDOWN
#endif  // LVGL_CPP_WIDGETS_DROPDOWN_H_
