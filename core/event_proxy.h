#ifndef LVGL_CPP_CORE_EVENT_PROXY_H_
#define LVGL_CPP_CORE_EVENT_PROXY_H_

#include <functional>

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
   * @param event_code The raw LVGL event code.
   * @param callback The callback function.
   * @return Reference to this proxy for chaining.
   */
  EventProxy& on(lv_event_code_t event_code, EventCallback callback);

  // Convenience shortcuts for common events
  EventProxy& on_clicked(EventCallback callback);
  EventProxy& on_pressed(EventCallback callback);
  EventProxy& on_released(EventCallback callback);
  EventProxy& on_value_changed(EventCallback callback);
  EventProxy& on_long_pressed(EventCallback callback);
  EventProxy& on_all(EventCallback callback);

  /**
   * @brief Add a callback for a specific event code (alias for on).
   * @param event_code The LVGL event code.
   * @param callback The callback function.
   * @return Reference to this proxy for chaining.
   */
  EventProxy& add_cb(lv_event_code_t event_code, EventCallback callback);
  EventProxy& add_cb(EventCode event_code, EventCallback callback);

  /**
   * @brief Manually send an event of a specific code to the target object.
   * @param code The event code.
   * @param param Optional user parameter.
   * @return Reference to this proxy for chaining.
   */
  EventProxy& send(lv_event_code_t code, void* param = nullptr);

  /**
   * @brief Remove all event callbacks from the target object.
   */
  void remove_all();

 private:
  Object* obj_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_EVENT_PROXY_H_
