#include "calendar.h"

#include "button_matrix.h"

#if LV_USE_CALENDAR

namespace lvgl {

Calendar::Calendar() : Calendar((Object*)nullptr) {}

Calendar::Calendar(Object* parent, Ownership ownership)
    : Object(lv_calendar_create(parent ? parent->raw() : nullptr), ownership) {}

Calendar::Calendar(lv_obj_t* obj, Ownership ownership)
    : Object(obj, ownership) {}

Calendar& Calendar::set_today_date(uint32_t year, uint32_t month,
                                   uint32_t day) {
  if (obj_) lv_calendar_set_today_date(obj_, year, month, day);
  return *this;
}

Calendar& Calendar::set_shown_date(uint32_t year, uint32_t month) {
  if (obj_) lv_calendar_set_month_shown(obj_, year, month);
  return *this;
}

Calendar& Calendar::set_highlighted_dates(lv_calendar_date_t highlighted[],
                                          size_t date_num) {
  if (obj_) lv_calendar_set_highlighted_dates(obj_, highlighted, date_num);
  return *this;
}

Calendar& Calendar::set_day_names(const char** day_names) {
  if (obj_) lv_calendar_set_day_names(obj_, day_names);
  return *this;
}

Calendar& Calendar::set_width(int32_t width) {
  Object::set_width(width);
  return *this;
}
Calendar& Calendar::set_height(int32_t height) {
  Object::set_height(height);
  return *this;
}
Calendar& Calendar::set_size(int32_t width, int32_t height) {
  Object::set_size(width, height);
  return *this;
}
Calendar& Calendar::align(lv_align_t align, int32_t x_ofs, int32_t y_ofs) {
  Object::align(align, x_ofs, y_ofs);
  return *this;
}
Calendar& Calendar::add_state(lv_state_t state) {
  Object::add_state(state);
  return *this;
}
Calendar& Calendar::remove_state(lv_state_t state) {
  Object::remove_state(state);
  return *this;
}
Calendar& Calendar::add_flag(lv_obj_flag_t flag) {
  Object::add_flag(flag);
  return *this;
}
Calendar& Calendar::remove_flag(lv_obj_flag_t flag) {
  Object::remove_flag(flag);
  return *this;
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

}  // namespace lvgl

#endif  // LV_USE_CALENDAR
