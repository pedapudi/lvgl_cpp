#include "calendar.h"

#include "button_matrix.h"

#if LV_USE_CALENDAR

namespace lvgl {

Calendar::Calendar()
    : Calendar(static_cast<Object*>(nullptr), Ownership::Managed) {}

Calendar::Calendar(Object* parent, Ownership ownership)
    : Widget(lv_calendar_create(parent ? parent->raw() : nullptr), ownership) {}

Calendar::Calendar(Object& parent) : Calendar(&parent) {}

Calendar::Calendar(lv_obj_t* obj, Ownership ownership)
    : Widget(obj, ownership) {}

Calendar& Calendar::set_today_date(uint32_t year, uint32_t month,
                                   uint32_t day) {
  if (raw()) lv_calendar_set_today_date(raw(), year, month, day);
  return *this;
}

Calendar& Calendar::set_shown_date(uint32_t year, uint32_t month) {
  if (raw()) lv_calendar_set_month_shown(raw(), year, month);
  return *this;
}

Calendar& Calendar::set_highlighted_dates(lv_calendar_date_t highlighted[],
                                          size_t date_num) {
  if (raw()) lv_calendar_set_highlighted_dates(raw(), highlighted, date_num);
  return *this;
}

Calendar& Calendar::set_day_names(const char** day_names) {
  if (raw()) lv_calendar_set_day_names(raw(), day_names);
  return *this;
}

ButtonMatrix Calendar::get_btnmatrix() {
  return ButtonMatrix(raw() ? lv_calendar_get_btnmatrix(raw()) : nullptr);
}

const lv_calendar_date_t* Calendar::get_today_date() {
  return raw() ? lv_calendar_get_today_date(raw()) : nullptr;
}

const lv_calendar_date_t* Calendar::get_showed_date() {
  return raw() ? lv_calendar_get_showed_date(raw()) : nullptr;
}

lv_calendar_date_t* Calendar::get_highlighted_dates() {
  return raw() ? lv_calendar_get_highlighted_dates(raw()) : nullptr;
}

size_t Calendar::get_highlighted_dates_num() {
  return raw() ? lv_calendar_get_highlighted_dates_num(raw()) : 0;
}

bool Calendar::get_pressed_date(lv_calendar_date_t* date) {
  return raw() ? lv_calendar_get_pressed_date(raw(), date) == LV_RESULT_OK
              : false;
}

lv_obj_t* Calendar::create_arrow_header() {
  return raw() ? lv_calendar_header_arrow_create(raw()) : nullptr;
}

lv_obj_t* Calendar::create_dropdown_header() {
  return raw() ? lv_calendar_header_dropdown_create(raw()) : nullptr;
}

Calendar& Calendar::on_value_changed(std::function<void(lvgl::Event&)> cb) {
  add_event_cb(LV_EVENT_VALUE_CHANGED, cb);
  return *this;
}

}  // namespace lvgl

#endif  // LV_USE_CALENDAR
