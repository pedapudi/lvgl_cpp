#ifndef LVGL_CPP_WIDGETS_LABEL_H_
#define LVGL_CPP_WIDGETS_LABEL_H_

#include <cstdint>
#include <string>

#include "../core/traits.h"
#include "../core/widget.h"  // IWYU pragma: export
#include "../misc/geometry.h"
#include "lvgl.h"  // IWYU pragma: export

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
class Label;
template <>
struct class_traits<Label> {
  static const lv_obj_class_t* get() { return &lv_label_class; }
};

class Subject;
class Observer;

/**
 * @brief Wrapper for lv_label.
 */
class Label : public Widget<Label> {
 public:
  /**
   * @brief Label long mode behavior.
   */
  enum class LongMode : uint8_t {
    Wrap = LV_LABEL_LONG_WRAP,
    Dot = LV_LABEL_LONG_DOT,
    Scroll = LV_LABEL_LONG_SCROLL,
    ScrollCircular = LV_LABEL_LONG_SCROLL_CIRCULAR,
    Clip = LV_LABEL_LONG_CLIP,
  };

  Label();
  explicit Label(Object* parent, Ownership ownership = Ownership::Default);
  explicit Label(Object& parent);
  explicit Label(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Create a new Label with text.
   * @param parent Parent object.
   * @param text The initial text.
   */
  Label(Object& parent, const std::string& text);

  /**
   * @brief Create a new Label with text.
   * @param parent Parent object.
   * @param text The initial text.
   */
  Label(Object& parent, const char* text);

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
  Label& set_text_fmt(const char* fmt, ...) LV_FORMAT_ATTRIBUTE(2, 3);

  /**
   * @brief Set the text using a format string and va_list.
   * @param fmt Format string.
   * @param args va_list of arguments.
   */
  Label& set_text_vfmt(const char* fmt, va_list args);

  /**
   * @brief Insert text at a position.
   * @param pos The position index.
   * @param txt The text to insert.
   */
  Label& ins_text(uint32_t pos, const char* txt);

  /**
   * @brief Cut text from a position.
   * @param pos The position index.
   * @param cnt The number of characters to cut.
   */
  Label& cut_text(uint32_t pos, uint32_t cnt);

  /**
   * @brief Set the text with a static string.
   * @param text The text to set. Must be kept alive by the caller.
   */
  Label& set_text_static(const char* text);

  /**
   * @brief Set the translation tag for the label.
   * @param state_id The state ID.
   * @param txt The text key.
   */
  Label& set_translation_tag(uint32_t state_id, const char* txt);

  /**
   * @brief Get the text of the label.
   * @return The text.
   */
  std::string get_text() const;

  /**
   * @brief Set the long mode behavior.
   * @param mode The long mode to set.
   */
  Label& set_long_mode(LongMode mode);

  /**
   * @brief Get the long mode behavior.
   * @return The current long mode.
   */
  LongMode get_long_mode() const;

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

  /**
   * @brief Check if text recoloring is enabled.
   * @return true if enabled.
   */
  bool get_recolor() const;

  /**
   * @brief Get the index of the letter at a point.
   * @param point_in The point relative to the label.
   * @return The index of the letter.
   */
  uint32_t get_letter_on(const Point& point_in) const;

  /**
   * @brief Check if a character is under a point.
   * @param pos The point relative to the label.
   * @return true if a character is under the point.
   */
  bool is_char_under_pos(const Point& pos) const;

  /**
   * @brief Get the position of a letter by index.
   * @param char_id The index of the letter.
   * @return The position of the letter (relative to the label).
   */
  Point get_letter_pos(uint32_t char_id) const;

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
