#ifndef LVGL_CPP_WIDGETS_ROLLER_H_
#define LVGL_CPP_WIDGETS_ROLLER_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "../core/widget.h"  // IWYU pragma: export
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

class Roller : public Widget<Roller> {
 public:
  using Widget::Widget;

  /**
   * @brief Roller modes.
   */
  enum class Mode : uint8_t {
    Normal = LV_ROLLER_MODE_NORMAL,
    Infinite = LV_ROLLER_MODE_INFINITE,
  };

  /**
   * @brief Create a Roller on the active screen.
   */
  Roller();

  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Roller(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a Roller with a parent.
   * @param parent The parent object.
   */
  explicit Roller(Object* parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a Roller with a parent (reference).
   */
  explicit Roller(Object& parent);

  Roller& set_options(const char* options, lv_roller_mode_t mode);
  Roller& set_options(const char* options, Mode mode);
  Roller& set_selected(uint32_t sel_opt, AnimEnable anim = AnimEnable::On);
  [[deprecated("Use set_selected(uint32_t, AnimEnable) instead")]]
  Roller& set_selected(uint32_t sel_opt, lv_anim_enable_t anim);
  bool set_selected_str(const char* sel_opt, AnimEnable anim = AnimEnable::On);
  [[deprecated("Use set_selected_str(const char*, AnimEnable) instead")]]
  bool set_selected_str(const char* sel_opt, lv_anim_enable_t anim);
  Roller& set_visible_row_count(uint32_t row_cnt);

  /**
   * @brief Register a callback for the ValueChanged event.
   * @param cb The callback function.
   */
  Roller& on_value_changed(std::function<void(lvgl::Event&)> cb);

  uint32_t get_selected();
  void get_selected_str(char* buf, uint32_t buf_size);
  const char* get_options();
  uint32_t get_option_count();
  lv_result_t get_option_str(uint32_t option, char* buf, uint32_t buf_size);

  // Deprecated legacy overloads
  Roller& set_options(const char* options, RollerMode mode) {
    return set_options(options, static_cast<Mode>(mode));
  }

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
