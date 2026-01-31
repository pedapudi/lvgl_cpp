#ifndef LVGL_CPP_INDEV_ENCODER_INPUT_H_
#define LVGL_CPP_INDEV_ENCODER_INPUT_H_

#include "../core/group.h"
#include "input_device.h"

namespace lvgl {

class EncoderInput : public InputDevice {
 public:
  using InputDevice::InputDevice;

  static EncoderInput create() {
    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_ENCODER);
    return EncoderInput(indev, Object::Ownership::Managed);
  }

  EncoderInput() : InputDevice() {}
  explicit EncoderInput(lv_indev_t* indev, Object::Ownership ownership =
                                               Object::Ownership::Default)
      : InputDevice(indev, ownership) {}

  EncoderInput(EncoderInput&& other) noexcept : InputDevice(std::move(other)) {}
  EncoderInput& operator=(EncoderInput&& other) noexcept {
    InputDevice::operator=(std::move(other));
    return *this;
  }

  void set_group(Group* group) {
    if (raw() && group) lv_indev_set_group(raw(), group->raw());
  }
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_ENCODER_INPUT_H_
