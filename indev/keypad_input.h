#ifndef LVGL_CPP_INDEV_KEYPAD_INPUT_H_
#define LVGL_CPP_INDEV_KEYPAD_INPUT_H_

#include "../core/group.h"
#include "input_device.h"

namespace lvgl {

class KeypadInput : public InputDevice {
 public:
  using InputDevice::InputDevice;

  static KeypadInput create() {
    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_KEYPAD);
    return KeypadInput(indev, true);
  }

  KeypadInput() : InputDevice() {}
  explicit KeypadInput(lv_indev_t* indev, bool owned = false)
      : InputDevice(indev, owned) {}

  KeypadInput(KeypadInput&& other) noexcept : InputDevice(std::move(other)) {}
  KeypadInput& operator=(KeypadInput&& other) noexcept {
    InputDevice::operator=(std::move(other));
    return *this;
  }

  // Inherits set_group from InputDevice, which is the primary configuration for
  // Keypad
  void set_group(Group* group) {
    if (raw() && group) lv_indev_set_group(raw(), group->raw());
  }
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_KEYPAD_INPUT_H_
