#ifndef LVGL_CPP_WIDGETS_LABEL_H_
#define LVGL_CPP_WIDGETS_LABEL_H_

#include "../core/object.h" // IWYU pragma: export
#include "lvgl.h" // IWYU pragma: export
#include <string>

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
  Label();
  explicit Label(Object *parent);

  /**
   * @brief Wrap an existing lv_label object.
   */
  explicit Label(lv_obj_t *obj);

  /**
   * @brief Set the text of the label.
   * @param text The text to set.
   */
  void set_text(const std::string &text);

  /**
   * @brief Set the text with C-style string.
   * @param text The text to set.
   */
  void set_text(const char *text);

  /**
   * @brief Set the text using printf-style formatting.
   * @param fmt Format string.
   */
  void set_text_fmt(const char *fmt, ...);

  /**
   * @brief Get the text of the label.
   * @return The text.
   */
  std::string get_text() const;

  /**
   * @brief Set the long mode behavior.
   * @param mode The long mode to set.
   */
  void set_long_mode(lv_label_long_mode_t mode);

  /**
   * @brief Get the long mode behavior.
   * @return The current long mode.
   */
  lv_label_long_mode_t get_long_mode() const;

  void set_selection_start(uint32_t index);
  void set_selection_end(uint32_t index);
  uint32_t get_selection_start() const;
  uint32_t get_selection_end() const;

  void set_recolor(bool en);
  bool get_recolor() const;

  /**
   * @brief Bind the label's text to a subject (Int, String, Pointer).
   * @param subject The subject to bind.
   * @param fmt Optional format string (e.g. "%d").
   * @return An Observer object. Keep this object alive to maintain the binding.
   */
  [[nodiscard]] Observer bind_text(Subject &subject, const char *fmt = nullptr);
};

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
} // namespace lvgl

#endif // LVGL_CPP_WIDGETS_LABEL_H_
