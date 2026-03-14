#ifndef LVGL_CPP_WIDGETS_ARCLABEL_H_
#define LVGL_CPP_WIDGETS_ARCLABEL_H_

#include <cstdint>
#include <string>

#include "../core/traits.h"
#include "../core/widget.h"
#include "lvgl.h"

#if LV_USE_ARCLABEL

namespace lvgl {

class ArcLabel;

template <>
struct class_traits<ArcLabel> {
  static const lv_obj_class_t* get() { return &lv_arclabel_class; }
};

/**
 * @brief Wrapper for lv_arclabel.
 *
 * ArcLabel allows displaying text along an arc.
 */
class ArcLabel : public Widget<ArcLabel> {
 public:
  /**
   * @brief Direction of the arc.
   */
  enum class Direction : uint8_t {
    Clockwise = LV_ARCLABEL_DIR_CLOCKWISE,
    CounterClockwise = LV_ARCLABEL_DIR_COUNTER_CLOCKWISE,
  };

  /**
   * @brief Text alignment relative to the arc.
   */
  enum class TextAlign : uint8_t {
    Default = LV_ARCLABEL_TEXT_ALIGN_DEFAULT,
    Leading = LV_ARCLABEL_TEXT_ALIGN_LEADING,
    Center = LV_ARCLABEL_TEXT_ALIGN_CENTER,
    Trailing = LV_ARCLABEL_TEXT_ALIGN_TRAILING,
  };

  /**
   * @brief Overflow behavior.
   */
  enum class Overflow : uint8_t {
    Visible = LV_ARCLABEL_OVERFLOW_VISIBLE,
    Ellipsis = LV_ARCLABEL_OVERFLOW_ELLIPSIS,
    Clip = LV_ARCLABEL_OVERFLOW_CLIP,
  };

  ArcLabel();
  explicit ArcLabel(Object* parent, Ownership ownership = Ownership::Default);
  explicit ArcLabel(Object& parent);
  explicit ArcLabel(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Set the text of the arc label.
   */
  ArcLabel& set_text(const std::string& text);
  ArcLabel& set_text(const char* text);
  ArcLabel& set_text_fmt(const char* fmt, ...) LV_FORMAT_ATTRIBUTE(2, 3);
  ArcLabel& set_text_static(const char* text);

  /**
   * @brief Get the text of the arc label.
   */
  const char* get_text() const;

  /**
   * @brief Set the start angle.
   */
  ArcLabel& set_angle_start(lv_value_precise_t start);
  lv_value_precise_t get_angle_start() const;

  /**
   * @brief Set the angle size.
   */
  ArcLabel& set_angle_size(lv_value_precise_t size);
  lv_value_precise_t get_angle_size() const;

  /**
   * @brief Set both start and size angles.
   */
  ArcLabel& set_angles(lv_value_precise_t start, lv_value_precise_t size);

  /**
   * @brief Set the rotation offset.
   */
  ArcLabel& set_offset(int32_t offset);

  /**
   * @brief Set the direction.
   */
  ArcLabel& set_dir(Direction dir);
  Direction get_dir() const;

  /**
   * @brief Set the radius.
   */
  ArcLabel& set_radius(uint32_t radius);
  uint32_t get_radius() const;

  /**
   * @brief Set center offsets.
   */
  ArcLabel& set_center_offset_x(uint32_t x);
  ArcLabel& set_center_offset_y(uint32_t y);
  uint32_t get_center_offset_x() const;
  uint32_t get_center_offset_y() const;

  /**
   * @brief Set text alignment.
   */
  ArcLabel& set_text_vertical_align(TextAlign align);
  ArcLabel& set_text_horizontal_align(TextAlign align);
  TextAlign get_text_vertical_align() const;
  TextAlign get_text_horizontal_align() const;

  /**
   * @brief Set overflow behavior.
   */
  ArcLabel& set_overflow(Overflow overflow);
  Overflow get_overflow() const;

  /**
   * @brief Set end overlap behavior.
   */
  ArcLabel& set_end_overlap(bool overlap);
  bool get_end_overlap() const;

  /**
   * @brief Enable text recoloring.
   */
  ArcLabel& set_recolor(bool en);
  bool get_recolor() const;

  /**
   * @brief Get the resulting text angle.
   */
  lv_value_precise_t get_text_angle() const;
};

}  // namespace lvgl

#endif  // LV_USE_ARCLABEL
#endif  // LVGL_CPP_WIDGETS_ARCLABEL_H_
