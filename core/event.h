#ifndef LVGL_CPP_CORE_EVENT_H_
#define LVGL_CPP_CORE_EVENT_H_

#include "../misc/enums.h"
#include "lvgl.h"

namespace lvgl {

class Object;  // Forward declaration to avoid circular dependency

/**
 * @brief Wrapper for lv_event_t to provide type-safe accessors.
 */
class Event {
 public:
  explicit Event(lv_event_t* e) : evt_(e) {}

  /**
   * @brief Get the event code.
   */
  EventCode get_code() const;

  /**
   * @brief Get the original target of the event.
   * @return An Unmanaged Object wrapper around the target.
   */
  Object get_target() const;

  /**
   * @brief Get the current target (the object determining the event handler).
   * @return An Unmanaged Object wrapper around the current target.
   */
  Object get_current_target() const;

  /**
   * @brief Get the user data associated with the event.
   */
  void* get_user_data() const;

  /**
   * @brief Get the parameter passed with the event.
   */
  void* get_param() const;

  /**
   * @brief Get the parameter cast to a specific type.
   */
  template <typename T>
  T* get_param() const {
    return static_cast<T*>(get_param());
  }

  /**
   * @brief Get the raw LVGL event pointer.
   */
  lv_event_t* raw() const { return evt_; }

 private:
  lv_event_t* evt_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_CORE_EVENT_H_
