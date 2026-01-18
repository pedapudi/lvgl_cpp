#ifndef LVGL_CPP_INDEV_BUTTON_INPUT_H_
#define LVGL_CPP_INDEV_BUTTON_INPUT_H_

#include <vector>

#include "input_device.h"

namespace lvgl {

class ButtonInput : public InputDevice {
 public:
  using InputDevice::InputDevice;

  static ButtonInput create() {
    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_BUTTON);
    return ButtonInput(indev, true);
  }

  ButtonInput() : InputDevice() {}
  explicit ButtonInput(lv_indev_t* indev, bool owned = false)
      : InputDevice(indev, owned) {}

  ButtonInput(ButtonInput&& other) noexcept
      : InputDevice(std::move(other)), points_(std::move(other.points_)) {
    // Re-register points if we moved?
    // lv_indev_set_button_points points to the address.
    // If std::vector moves, the data pointer might stay valid or change?
    // Actually std::vector move preserves the buffer, so the address is Valid.
    // BUT if we assign to a new object, the pointers inside LVGL need to be
    // valid. The implementation of vector move usually swaps pointers. So the
    // data address remains same? Yes.
  }

  ButtonInput& operator=(ButtonInput&& other) noexcept {
    if (this != &other) {
      InputDevice::operator=(std::move(other));
      points_ = std::move(other.points_);
      // No need to re-call lv_indev_set_button_points if vector buffer didn't
      // change location? Wait, if we move-assign, 'other' loses its buffer.
      // 'points_' gains it. The pointer held by LVGL must point to the valid
      // buffer. Since vector move transfers ownership of the buffer, the
      // address of data usually stays same. So LVGL continues to point to valid
      // memory.
    }
    return *this;
  }

  void set_points(const std::vector<lv_point_t>& points) {
    points_ = points;  // Copy to our storage
    if (raw()) {
      lv_indev_set_button_points(raw(), points_.data());
    }
  }

 private:
  std::vector<lv_point_t> points_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_BUTTON_INPUT_H_
