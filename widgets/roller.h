#ifndef LVGL_CPP_WIDGETS_ROLLER_H_
#define LVGL_CPP_WIDGETS_ROLLER_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_ROLLER
/**
 * @file roller.h
 * @brief C++ Wrapper for LVGL Roller Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Roller widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {
class Subject;
class Observer;

class Roller : public Object {
 public:
  /**
   * @brief Create a Roller on the active screen.
   */
  Roller();
  /**
   * @brief Create a Roller with a parent.
   * @param parent The parent object.
   */
  explicit Roller(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Roller(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Roller& set_options(const char* options, lv_roller_mode_t mode);
  Roller& set_selected(uint32_t sel_opt, lv_anim_enable_t anim = LV_ANIM_ON);
  bool set_selected_str(const char* sel_opt,
                        lv_anim_enable_t anim = LV_ANIM_ON);
  Roller& set_visible_row_count(uint32_t row_cnt);

  // Fluent API shadows
  Roller& set_width(int32_t width);
  Roller& set_height(int32_t height);
  Roller& set_size(int32_t width, int32_t height);
  Roller& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Roller& add_state(lv_state_t state);
  Roller& remove_state(lv_state_t state);
  Roller& add_flag(lv_obj_flag_t flag);
  Roller& remove_flag(lv_obj_flag_t flag);

  uint32_t get_selected();
  void get_selected_str(char* buf, uint32_t buf_size);
  const char* get_options();
  uint32_t get_option_count();
  lv_result_t get_option_str(uint32_t option, char* buf, uint32_t buf_size);

  /**
   * @brief Bind the roller's selected option to an integer subject.
   * @param subject The integer subject to bind.
   * @return An Observer object. Keep this object alive to maintain the binding.
   */
  [[nodiscard]] Observer bind_value(Subject& subject);
};

/**
 * @file roller.h
 * @brief C++ Wrapper for LVGL Roller Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Roller widget(lv_screen_active());
 * widget.center();
 * ```
 */
}  // namespace lvgl

#endif  // LV_USE_ROLLER
#endif  // LVGL_CPP_WIDGETS_ROLLER_H_
