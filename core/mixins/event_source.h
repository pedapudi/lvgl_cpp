#pragma once

#include <functional>
#include <memory>
#include <vector>

#include "../event.h"

namespace lvgl {

template <typename Derived>
class EventSource {
 public:
  using EventCallback = std::function<void(Event& event)>;

  Derived& add_event_cb(EventCallback cb, lv_event_code_t filter) {
    // Delegate to Object's managed event system to ensure cleanup.
    // Derived must assume Object is a base class.
    auto obj = static_cast<Derived*>(this);
    obj->Object::add_event_cb(filter, cb);
    return *obj;
  }

  Derived& on_click(EventCallback cb) {
    return add_event_cb(cb, LV_EVENT_CLICKED);
  }

  Derived& on_clicked(EventCallback cb) {
    return add_event_cb(cb, LV_EVENT_CLICKED);
  }

  Derived& on_event(EventCallback cb) { return add_event_cb(cb, LV_EVENT_ALL); }

  Derived& on_pressed(EventCallback cb) {
    return add_event_cb(cb, LV_EVENT_PRESSED);
  }

  Derived& on_released(EventCallback cb) {
    return add_event_cb(cb, LV_EVENT_RELEASED);
  }

  Derived& on_long_pressed(EventCallback cb) {
    return add_event_cb(cb, LV_EVENT_LONG_PRESSED);
  }
};

}  // namespace lvgl
