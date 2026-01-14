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
  explicit Calendar(Object* parent, Ownership ownership = Ownership::Default);
  /**
   * @brief Wrap an existing lv_obj object.
   * @param obj The raw LVGL object to wrap.
   */
  explicit Calendar(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  void set_today_date(uint32_t year, uint32_t month, uint32_t day);
  void set_shown_date(uint32_t year, uint32_t month);
  void set_highlighted_dates(lv_calendar_date_t highlighted[], size_t date_num);
  void set_day_names(const char** day_names);

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


#endif // LV_USE_CALENDAR
#endif  // LVGL_CPP_WIDGETS_CALENDAR_H_
