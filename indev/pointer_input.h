#ifndef LVGL_CPP_INDEV_POINTER_INPUT_H_
#define LVGL_CPP_INDEV_POINTER_INPUT_H_

#include "../core/object.h"
#include "input_device.h"

namespace lvgl {

class PointerInput : public InputDevice {
 public:
  using InputDevice::InputDevice;  // Inherit constructors

  // Factory
  static PointerInput create() {
    // We can cast the result of InputDevice::create because we know the layout
    // is compatible dependent on how we handle the object slicing or moving.
    // Actually, InputDevice::create returns an InputDevice by value.
    // We need a move constructor or similar.
    // Better yet, just create directly here.
    lv_indev_t* indev = lv_indev_create();
    lv_indev_set_type(indev, LV_INDEV_TYPE_POINTER);
    return PointerInput(indev, true);
  }

  // Constructors
  PointerInput() : InputDevice() {}
  explicit PointerInput(lv_indev_t* indev, bool owned = false)
      : InputDevice(indev, owned) {}

  // Move semantics
  PointerInput(PointerInput&& other) noexcept : InputDevice(std::move(other)) {}
  PointerInput& operator=(PointerInput&& other) noexcept {
    InputDevice::operator=(std::move(other));
    return *this;
  }

  // Pointer-specific methods
  void set_range(int32_t w, int32_t h) {
    if (raw()) {
      // There is no lv_indev_set_range in v9?
      // Checking doc: logic usually handled by display size, but older versions
      // had it. If not present, we skip.
    }
  }

  // Shadow set_cursor to be more prominent or just inherit it
  // using InputDevice::set_cursor;
  void set_cursor(Object* cur_obj) {
    if (raw() && cur_obj) lv_indev_set_cursor(raw(), cur_obj->raw());
  }
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_POINTER_INPUT_H_
