#ifndef LVGL_CPP_WIDGETS_LABEL_H_
#define LVGL_CPP_WIDGETS_LABEL_H_

#include <cstdint>
#include <string>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_LABEL
/**
 * @file label.h
 * @brief C++ Wrapper for LVGL Label Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Label widget(lv_screen_active());
 * widget.center();
 * ```
 */
namespace lvgl {
class Subject;
class Observer;

/**
 * @brief Wrapper for lv_label.
 */
class Label : public Object {
 public:
  /**
   * @brief Create a new Label.
   * @param parent Parent object.
   */
  /**
   * @brief Create a new Label.
   * @param parent Parent object.
   * @param ownership Ownership policy.
   */
  Label();
  explicit Label(Object* parent, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a new Label with text.
   * @param parent Parent object.
   * @param text The initial text.
   */
  Label(Object* parent, const std::string& text);

  /**
   * @brief Create a new Label with text.
   * @param parent Parent object.
   * @param text The initial text.
   */
  Label(Object* parent, const char* text);

  /**
   * @brief Wrap an existing lv_label object.
   * @param obj The raw LVGL object to wrap.
   * @param ownership Ownership policy.
   */
  explicit Label(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Set the text of the label.
   * @param text The text to set.
   */
  Label& set_text(const std::string& text);

  /**
   * @brief Set the text with C-style string.
   * @param text The text to set.
   */
  Label& set_text(const char* text);

  /**
   * @brief Set the text using printf-style formatting.
   * @param fmt Format string.
   */
  Label& set_text_fmt(const char* fmt, ...);

  /**
   * @brief Get the text of the label.
   * @return The text.
   */
  std::string get_text() const;

  /**
   * @brief Set the long mode behavior.
   * @param mode The long mode to set.
   */
  Label& set_long_mode(lv_label_long_mode_t mode);

  /**
   * @brief Get the long mode behavior.
   * @return The current long mode.
   */
  lv_label_long_mode_t get_long_mode() const;

  /**
   * @brief Set the selection start index.
   * @param index Character index.
   */
  Label& set_selection_start(uint32_t index);

  /**
   * @brief Set the selection end index.
   * @param index Character index.
   */
  Label& set_selection_end(uint32_t index);

  /**
   * @brief Get the selection start index.
   * @return The selection start index.
   */
  uint32_t get_selection_start() const;

  /**
   * @brief Get the selection end index.
   * @return The selection end index.
   */
  uint32_t get_selection_end() const;

  /**
   * @brief Enable or disable text recoloring commands.
   * @param en true to enable.
   */
  Label& set_recolor(bool en);

  // Fluent API shadows for common Object methods
  Label& set_width(int32_t width);
  Label& set_height(int32_t height);
  Label& set_size(int32_t width, int32_t height);
  Label& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);

  /**
   * @brief Check if text recoloring is enabled.
   * @return true if enabled.
   */
  bool get_recolor() const;

  /**
   * @brief Bind the label's text to a subject (Int, String, Pointer).
   * @param subject The subject to bind.
   * @param fmt Optional format string (e.g. "%d").
   * @return An Observer object. Keep this object alive to maintain the binding.
   */
  [[nodiscard]] Observer bind_text(Subject& subject, const char* fmt = nullptr);
};

}  // namespace lvgl

#endif  // LV_USE_LABEL
#endif  // LVGL_CPP_WIDGETS_LABEL_H_
