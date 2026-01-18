#pragma once

#include "../event.h"
#include <functional>
#include <vector>
#include <memory>

namespace lvgl {

template <typename Derived>
class EventSource {
 public:
  using EventCallback = std::function<void(Event& event)>;

  Derived& add_event_cb(EventCallback cb, lv_event_code_t filter) {
    auto wrapper = new EventCallbackWrapper{cb};
    lv_obj_add_event_cb(static_cast<Derived*>(this)->raw(), event_handler, filter, wrapper);
    return *static_cast<Derived*>(this);
  }

  Derived& on_click(EventCallback cb) {
      return add_event_cb(cb, LV_EVENT_CLICKED);
  }

  Derived& on_event(EventCallback cb) {
      return add_event_cb(cb, LV_EVENT_ALL);
  }

 private:
  struct EventCallbackWrapper {
    EventCallback callback;
  };

  static void event_handler(lv_event_t* e) {
    auto wrapper = static_cast<EventCallbackWrapper*>(lv_event_get_user_data(e));
    if (wrapper && wrapper->callback) {
      Event event(e);
      wrapper->callback(event);
    }
  }
};

} // namespace lvgl
