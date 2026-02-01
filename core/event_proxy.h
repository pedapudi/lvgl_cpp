#ifndef LVGL_CPP_CORE_EVENT_PROXY_H_
#define LVGL_CPP_CORE_EVENT_PROXY_H_

#include <functional>

#include "../indev/gesture_event.h"
#include "../misc/enums.h"
#include "lvgl.h"

namespace lvgl {

class Object;
class Event;

/**
 * @brief Proxy for managing event callbacks on an object.
 * Provides a fluent API for adding and removing event listeners.
 */
class EventProxy {
 public:
  using EventCallback = std::function<void(Event&)>;

  explicit EventProxy(Object* obj);

  /**
   * @brief Generic method to add a callback for ANY LVGL event code.
   */
  EventProxy& on(EventCode event_code, EventCallback callback);

  // Convenience shortcuts for common events - Inlined for Performance
  EventProxy& on_clicked(EventCallback callback) {
    return on(LV_EVENT_CLICKED, std::move(callback));
  }
  EventProxy& on_pressed(EventCallback callback) {
    return on(LV_EVENT_PRESSED, std::move(callback));
  }
  EventProxy& on_released(EventCallback callback) {
    return on(LV_EVENT_RELEASED, std::move(callback));
  }
  EventProxy& on_value_changed(EventCallback callback) {
    return on(LV_EVENT_VALUE_CHANGED, std::move(callback));
  }
  EventProxy& on_long_pressed(EventCallback callback) {
    return on(LV_EVENT_LONG_PRESSED, std::move(callback));
  }
  EventProxy& on_all(EventCallback callback) {
    return on(LV_EVENT_ALL, std::move(callback));
  }

  EventProxy& on_gesture(std::function<void(GestureEvent&)> callback);

  /**
   * @brief Add a callback for a specific event code (alias for on).
   */
  EventProxy& add_cb(EventCode event_code, EventCallback callback) {
    return on(event_code, std::move(callback));
  }

  /**
   * @brief Manually send an event of a specific code to the target object.
   */
  EventProxy& send(EventCode code, void* param = nullptr);

  /**
   * @brief Remove all event callbacks from the target object.
   */
  void remove_all();

 private:
  Object* obj_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_EVENT_PROXY_H_
