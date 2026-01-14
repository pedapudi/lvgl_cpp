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
  explicit Slider(Object* parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Wrap an existing lv_slider object.
   */
  explicit Slider(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  // Inherits set_value, set_range, etc. from Bar

  /**
   * @brief Set the value of the left knob (for range slider).
   * @param value The value to set.
   * @param anim Enable animation (`LV_ANIM_ON`/`LV_ANIM_OFF`).
   */
  void set_left_value(int32_t value, lv_anim_enable_t anim = LV_ANIM_ON);

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
