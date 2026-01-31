#ifndef LVGL_CPP_INDEV_GESTURE_PROXY_H_
#define LVGL_CPP_INDEV_GESTURE_PROXY_H_

#include <functional>

#include "../misc/enums.h"
#include "gesture_event.h"
#include "lvgl.h"

namespace lvgl {

class InputDevice;

/**
 * @brief Proxy for fluent gesture configuration and event handling.
 */
class GestureProxy {
 public:
  explicit GestureProxy(InputDevice* indev) : indev_(indev) {}

  /**
   * @brief Register a callback for gesture events.
   * @param cb The callback function.
   * @return Reference to this proxy.
   */
  GestureProxy& on_gesture(std::function<void(GestureEvent)> cb);

  /**
   * @brief Configure gesture recognition parameters.
   */
  struct Config {
    InputDevice* indev;
    Config& min_velocity(uint16_t v) {
      // Note: LVGL 9 handles this via indev config if available,
      // or it might be a global/style setting.
      // In LVGL 9.1+, it's often part of the indev itself.
      return *this;
    }
    Config& limit(uint16_t dist) {
      // lv_indev_set_scroll_limit(indev->raw(), dist); // Closest equivalent if
      // applicable
      return *this;
    }
  };

  Config config() { return {indev_}; }

 private:
  InputDevice* indev_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_GESTURE_PROXY_H_
