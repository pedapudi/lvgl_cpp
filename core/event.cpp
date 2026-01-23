#include "event.h"

#include "object.h"

namespace lvgl {

EventCode Event::get_code() const {
  return static_cast<EventCode>(lv_event_get_code(evt_));
}

Object Event::get_target() const {
  return Object(static_cast<lv_obj_t*>(lv_event_get_target(evt_)),
                Object::Ownership::Unmanaged);
}

Object Event::get_current_target() const {
  return Object(static_cast<lv_obj_t*>(lv_event_get_current_target(evt_)),
                Object::Ownership::Unmanaged);
}

void* Event::get_user_data() const { return lv_event_get_user_data(evt_); }

void* Event::get_param() const { return lv_event_get_param(evt_); }

void Event::stop_bubbling() { lv_event_stop_bubbling(evt_); }

void Event::stop_processing() { lv_event_stop_processing(evt_); }

}  // namespace lvgl
