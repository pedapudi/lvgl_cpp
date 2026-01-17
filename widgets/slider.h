#ifndef LVGL_CPP_WIDGETS_SLIDER_H_
#define LVGL_CPP_WIDGETS_SLIDER_H_

#include <cstdint>

#include "bar.h"   // IWYU pragma: export
#include "lvgl.h"  // IWYU pragma: export

#if LV_USE_SLIDER
/**
 * @file slider.h
 * @brief C++ Wrapper for LVGL Slider Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Slider widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {
class Subject;
class Observer;

/**
 * @brief Wrapper for lv_slider.
 */
class Slider : public Bar {
 public:
  /**
   * @brief Create a new Slider.
   * @param parent Parent object.
   */
  Slider();
  explicit Slider(Object& parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a Slider with min and max values.
   * @param parent Parent object.
   * @param min Minimum value.
   * @param max Maximum value.
   */
  Slider(Object& parent, int32_t min, int32_t max);

  /**
   * @brief Wrap an existing lv_slider object.
   */
  explicit Slider(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  // Inherits set_value, set_range, etc. from Bar, but we shadow them for fluent
  // API
  Slider& set_value(int32_t value, lv_anim_enable_t anim = LV_ANIM_ON);
  Slider& set_start_value(int32_t value, lv_anim_enable_t anim = LV_ANIM_ON);
  Slider& set_range(int32_t min, int32_t max);
  Slider& set_mode(lv_bar_mode_t mode);

  /**
   * @brief Set the value of the left knob (for range slider).
   * @param value The value to set.
   * @param anim Enable animation (`LV_ANIM_ON`/`LV_ANIM_OFF`).
   */
  Slider& set_left_value(int32_t value, lv_anim_enable_t anim = LV_ANIM_ON);

  // Fluent API shadows for Object
  Slider& set_width(int32_t width);
  Slider& set_height(int32_t height);
  Slider& set_size(int32_t width, int32_t height);
  Slider& align(Align align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Slider& add_state(lv_state_t state);
  Slider& remove_state(lv_state_t state);
  Slider& add_flag(lv_obj_flag_t flag);
  Slider& remove_flag(lv_obj_flag_t flag);

  /**
   * @brief Get the value of the left knob.
   * @return The value.
   */
  int32_t get_left_value() const;

  /**
   * @brief Check if the slider is currently being dragged.
   * @return true if dragged.
   */
  bool is_dragged() const;

  /**
   * @brief Bind the slider's value to an integer subject.
   * @param subject The integer subject to bind.
   * @return An Observer object. Keep this object alive to maintain the binding.
   */
  [[nodiscard]] Observer bind_value(Subject& subject);
};

}  // namespace lvgl

#endif  // LV_USE_SLIDER
#endif  // LVGL_CPP_WIDGETS_SLIDER_H_
