#ifndef LVGL_CPP_INDEV_GESTURE_EVENT_H_
#define LVGL_CPP_INDEV_GESTURE_EVENT_H_

#include "../core/event.h"
#include "../misc/enums.h"
#include "../misc/geometry.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Wrapper for gesture events.
 */
class GestureEvent : public Event {
 public:
  explicit GestureEvent(lv_event_t* event) : Event(event) {}

  /**
   * @brief Get the gesture type.
   */
  Dir get_dir() const {
    return static_cast<Dir>(
        lv_indev_get_gesture_dir(lv_event_get_indev(raw())));
  }

  /**
   * @brief Get the velocity of the gesture.
   */
  Point get_velocity() const {
    lv_point_t p;
    lv_indev_get_vect(lv_event_get_indev(raw()), &p);
    return Point(p.x, p.y);
  }
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_GESTURE_EVENT_H_
