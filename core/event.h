#ifndef LVGL_CPP_CORE_EVENT_H_
#define LVGL_CPP_CORE_EVENT_H_

#include "../misc/enums.h"
#include "lvgl.h"

/**
 * @file event.h
 * @brief User Guide:
 * The `Event` class is a lightweight wrapper around `lv_event_t`. It is
 * typically received in event callbacks added via `Object::add_event_cb`.
 *
 * Key Features:
 * - **Target Access**: Safely retrieve the triggering object or the handling
 * object as an `Object` wrapper.
 * - **Type-Safe Codes**: Access event codes using the `EventCode` enum.
 * - **Propagation Control**: Methods to stop bubbling or further processing.
 * - **Parameter Handling**: Template methods to retrieve and cast event
 * parameters.
 */

namespace lvgl {

class Object;

/**
 * @brief C++ wrapper for LVGL events.
 */
class Event {
 public:
  explicit Event(lv_event_t* e) : evt_(e) {}

  /**
   * @brief Get the event code.
   * @return The event code as an EventCode enum.
   */
  EventCode get_code() const;

  /**
   * @brief Get the original target of the event.
   * @return An Unmanaged Object wrapper around the target.
   */
  Object get_target() const;

  template <typename T>
  T get_target() const;

  /**
   * @brief Get the current target (the object determining the event handler).
   * @return An Unmanaged Object wrapper around the current target.
   */
  Object get_current_target() const;

  template <typename T>
  T get_current_target() const;

  /**
   * @brief Get the user data associated with the event.
   * @return Pointer to user data.
   */
  void* get_user_data() const;

  /**
   * @brief Get the parameter passed with the event.
   * @return Pointer to parameter.
   */
  void* get_param() const;

  /**
   * @brief Get the parameter cast to a specific type.
   * @tparam T The type to cast to.
   * @return Pointer to parameter cast to type T.
   */
  template <typename T>
  T* get_param() const {
    return static_cast<T*>(get_param());
  }

  /**
   * @brief Stop the event from propagating to parent objects.
   */
  void stop_bubbling();

  /**
   * @brief Stop further event handlers from being called for this event.
   */
  void stop_processing();

  /**
   * @brief Get the raw LVGL event pointer.
   */
  lv_event_t* raw() const { return evt_; }

 private:
  lv_event_t* evt_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_EVENT_H_
