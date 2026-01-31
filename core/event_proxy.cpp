#include "event_proxy.h"

#include "event.h"
#include "object.h"

namespace lvgl {

EventProxy::EventProxy(Object* obj) : obj_(obj) {}

EventProxy& EventProxy::on(lv_event_code_t event_code, EventCallback callback) {
  if (obj_) obj_->add_event_cb(event_code, std::move(callback));
  return *this;
}

EventProxy& EventProxy::on_clicked(EventCallback callback) {
  return on(LV_EVENT_CLICKED, std::move(callback));
}

EventProxy& EventProxy::on_pressed(EventCallback callback) {
  return on(LV_EVENT_PRESSED, std::move(callback));
}

EventProxy& EventProxy::on_released(EventCallback callback) {
  return on(LV_EVENT_RELEASED, std::move(callback));
}

EventProxy& EventProxy::on_value_changed(EventCallback callback) {
  return on(LV_EVENT_VALUE_CHANGED, std::move(callback));
}

EventProxy& EventProxy::on_long_pressed(EventCallback callback) {
  return on(LV_EVENT_LONG_PRESSED, std::move(callback));
}

EventProxy& EventProxy::on_all(EventCallback callback) {
  return on(LV_EVENT_ALL, std::move(callback));
}

EventProxy& EventProxy::on_gesture(
    std::function<void(GestureEvent&)> callback) {
  return on(LV_EVENT_GESTURE, [callback](Event& e) {
    GestureEvent ge(e.raw());
    callback(ge);
  });
}

EventProxy& EventProxy::add_cb(lv_event_code_t event_code,
                               EventCallback callback) {
  return on(event_code, std::move(callback));
}

EventProxy& EventProxy::add_cb(EventCode event_code, EventCallback callback) {
  return add_cb(static_cast<lv_event_code_t>(event_code), std::move(callback));
}

EventProxy& EventProxy::send(lv_event_code_t code, void* param) {
  if (obj_) lv_obj_send_event(obj_->raw(), code, param);
  return *this;
}

void EventProxy::remove_all() {
  if (obj_) obj_->remove_all_event_cbs();
}

}  // namespace lvgl
