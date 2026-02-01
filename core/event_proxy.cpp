#include "event_proxy.h"

#include "event.h"
#include "object.h"

namespace lvgl {

EventProxy::EventProxy(Object* obj) : obj_(obj) {}

EventProxy& EventProxy::on(EventCode event_code, EventCallback callback) {
  if (obj_)
    obj_->add_event_cb(static_cast<lv_event_code_t>(event_code),
                       std::move(callback));
  return *this;
}

EventProxy& EventProxy::on_gesture(
    std::function<void(GestureEvent&)> callback) {
  return on(EventCode::Gesture, [callback](Event& e) {
    GestureEvent ge(e.raw());
    callback(ge);
  });
}

EventProxy& EventProxy::send(EventCode code, void* param) {
  if (obj_ && obj_->raw())
    lv_obj_send_event(obj_->raw(), static_cast<lv_event_code_t>(code), param);
  return *this;
}

void EventProxy::remove_all() {
  if (obj_) obj_->remove_all_event_cbs();
}

}  // namespace lvgl
