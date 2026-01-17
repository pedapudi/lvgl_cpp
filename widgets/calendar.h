#ifndef LVGL_CPP_WIDGETS_CALENDAR_H_
#define LVGL_CPP_WIDGETS_CALENDAR_H_

#include <cstdint>

#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

#if LV_USE_CALENDAR
/**
 * @file calendar.h
 * @brief C++ Wrapper for LVGL Calendar Widget.
 *
 * # Usage
 *
 * ```cpp
 * lvgl::Calendar widget(lv_screen_active());
 * widget.center();
 * lvgl::ButtonMatrix btnm = widget.get_btnmatrix();
 * ```
 */
namespace lvgl {

class ButtonMatrix;
class Calendar : public Object {
 public:
  /**
   * @brief Create a Calendar on the active screen.
   */
  Calendar();
  /**
   * @brief Create a Calendar with a parent.
   * @param parent The parent object.
   */
  explicit Calendar(Object& parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Calendar(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  Calendar& set_today_date(uint32_t year, uint32_t month, uint32_t day);
  Calendar& set_shown_date(uint32_t year, uint32_t month);
  Calendar& set_highlighted_dates(lv_calendar_date_t highlighted[],
                                  size_t date_num);
  Calendar& set_day_names(const char** day_names);

  // Fluent API shadows
  Calendar& set_width(int32_t width);
  Calendar& set_height(int32_t height);
  Calendar& set_size(int32_t width, int32_t height);
  Calendar& align(lv_align_t align, int32_t x_ofs = 0, int32_t y_ofs = 0);
  Calendar& add_state(lv_state_t state);
  Calendar& remove_state(lv_state_t state);
  Calendar& add_flag(lv_obj_flag_t flag);
  Calendar& remove_flag(lv_obj_flag_t flag);

  ButtonMatrix get_btnmatrix();
  const lv_calendar_date_t* get_today_date();
  const lv_calendar_date_t* get_showed_date();
  lv_calendar_date_t* get_highlighted_dates();
  size_t get_highlighted_dates_num();
  bool get_pressed_date(lv_calendar_date_t* date);

  // Header helpers (require lv_calendar_header_arrow/dropdown.h which are
  // usually included)
  lv_obj_t* create_arrow_header();
  lv_obj_t* create_dropdown_header();
};

}  // namespace lvgl

#endif  // LV_USE_CALENDAR
#endif  // LVGL_CPP_WIDGETS_CALENDAR_H_
