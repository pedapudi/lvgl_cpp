#ifndef LVGL_CPP_INDEV_ENCODER_INPUT_H_
#define LVGL_CPP_INDEV_ENCODER_INPUT_H_

#include "input_device.h"

namespace lvgl {

class EncoderInput : public InputDevice {
 public:
  using InputDevice::InputDevice;

  static EncoderInput create() {
    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_ENCODER);
    return EncoderInput(indev, true);
  }

  EncoderInput() : InputDevice() {}
  explicit EncoderInput(lv_indev_t* indev, bool owned = false)
      : InputDevice(indev, owned) {}

  EncoderInput(EncoderInput&& other) noexcept : InputDevice(std::move(other)) {}
  EncoderInput& operator=(EncoderInput&& other) noexcept {
    InputDevice::operator=(std::move(other));
    return *this;
  }
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_ENCODER_INPUT_H_
