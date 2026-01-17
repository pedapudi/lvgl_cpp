#include "event.h"

#include "object.h"

namespace lvgl {

lv_event_code_t Event::get_code() const { return lv_event_get_code(evt_); }

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

}  // namespace lvgl
