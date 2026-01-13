#include "calendar.h"
#include "button_matrix.h"

namespace lvgl {

Calendar::Calendar() : Calendar((Object *)nullptr) {}

Calendar::Calendar(Object* parent)
    : Object(lv_calendar_create(parent ? parent->raw() : nullptr)) {}

Calendar::Calendar(lv_obj_t* obj) : Object(obj) {}

void Calendar::set_today_date(uint32_t year, uint32_t month, uint32_t day) {
  if (obj_)
    lv_calendar_set_today_date(obj_, year, month, day);
}

void Calendar::set_shown_date(uint32_t year, uint32_t month) {
  if (obj_)
    lv_calendar_set_month_shown(obj_, year, month);
}

void Calendar::set_highlighted_dates(lv_calendar_date_t highlighted[],
                                     size_t date_num) {
  if (obj_)
    lv_calendar_set_highlighted_dates(obj_, highlighted, date_num);
}

void Calendar::set_day_names(const char **day_names) {
  if (obj_)
    lv_calendar_set_day_names(obj_, day_names);
}

ButtonMatrix Calendar::get_btnmatrix() {
  return ButtonMatrix(obj_ ? lv_calendar_get_btnmatrix(obj_) : nullptr);
}

const lv_calendar_date_t* Calendar::get_today_date() {
  return obj_ ? lv_calendar_get_today_date(obj_) : nullptr;
}

const lv_calendar_date_t* Calendar::get_showed_date() {
  return obj_ ? lv_calendar_get_showed_date(obj_) : nullptr;
}

lv_calendar_date_t* Calendar::get_highlighted_dates() {
  return obj_ ? lv_calendar_get_highlighted_dates(obj_) : nullptr;
}

size_t Calendar::get_highlighted_dates_num() {
  return obj_ ? lv_calendar_get_highlighted_dates_num(obj_) : 0;
}

bool Calendar::get_pressed_date(lv_calendar_date_t* date) {
  return obj_ ? lv_calendar_get_pressed_date(obj_, date) == LV_RESULT_OK
              : false;
}

lv_obj_t* Calendar::create_arrow_header() {
  return obj_ ? lv_calendar_header_arrow_create(obj_) : nullptr;
}

lv_obj_t* Calendar::create_dropdown_header() {
  return obj_ ? lv_calendar_header_dropdown_create(obj_) : nullptr;
}

} // namespace lvgl
