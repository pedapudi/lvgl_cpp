#include "input_device.h"

namespace lvgl {

// Static trampoline to bridge C -> C++
static void cpp_read_cb_trampoline(lv_indev_t* indev, lv_indev_data_t* data) {
  auto* instance = static_cast<InputDevice*>(lv_indev_get_user_data(indev));
  if (instance) {
    instance->process_read(data);
  }
}

InputDevice::InputDevice() : indev_(nullptr), owned_(false) {}

InputDevice::InputDevice(lv_indev_t* indev, bool owned)
    : indev_(indev), owned_(owned) {}

InputDevice::~InputDevice() {
  if (owned_ && indev_) {
    lv_indev_delete(indev_);
  }
}

InputDevice::InputDevice(InputDevice&& other) noexcept
    : indev_(other.indev_),
      owned_(other.owned_),
      read_cb_(std::move(other.read_cb_)) {
  other.indev_ = nullptr;
  other.owned_ = false;
  // Update user data to point to the new address if we are the owner / handler
  if (indev_) {
    lv_indev_set_user_data(indev_, this);
  }
}

InputDevice& InputDevice::operator=(InputDevice&& other) noexcept {
  if (this != &other) {
    if (owned_ && indev_) {
      lv_indev_delete(indev_);
    }
    indev_ = other.indev_;
    owned_ = other.owned_;
    read_cb_ = std::move(other.read_cb_);
    other.indev_ = nullptr;
    other.owned_ = false;
    if (indev_) {
      lv_indev_set_user_data(indev_, this);
    }
  }
  return *this;
}

InputDevice InputDevice::create(lv_indev_type_t type) {
  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, type);
  return InputDevice(indev, true);
}

InputDevice* InputDevice::get_act() {
  static InputDevice instance(nullptr, false);
  // This is tricky: get_act returns a wrapper around the active indev.
  // It is NOT the original C++ object if one exists.
  // We can check user_data, but for safety, we return a temporary-ish wrapper.
  // Ideally, this returns a pointer to the singleton wrapper.
  // For now, let's keep the existing pattern but be aware it's unowned.
  lv_indev_t* act = lv_indev_active();
  if (!act) return nullptr;

  // Re-use the static instance to wrap the current active device
  instance = InputDevice(act, false);
  return &instance;
}

void InputDevice::set_read_cb(std::function<void(lv_indev_data_t*)> cb) {
  read_cb_ = cb;
  read_cb_v2_ = nullptr;
  if (indev_) {
    lv_indev_set_user_data(indev_, this);
    lv_indev_set_read_cb(indev_, cpp_read_cb_trampoline);
  }
}

void InputDevice::set_read_cb(std::function<void(IndevData&)> cb) {
  read_cb_v2_ = cb;
  read_cb_ = nullptr;
  if (indev_) {
    lv_indev_set_user_data(indev_, this);
    lv_indev_set_read_cb(indev_, cpp_read_cb_trampoline);
  }
}

void InputDevice::process_read(lv_indev_data_t* data) {
  if (read_cb_v2_) {
    IndevData wrapped(data);
    read_cb_v2_(wrapped);
  } else if (read_cb_) {
    read_cb_(data);
  }
}

void InputDevice::set_type(lv_indev_type_t type) {
  if (indev_) lv_indev_set_type(indev_, type);
}

// ... wrapped methods ...

IndevType InputDevice::get_type() const {
  return indev_ ? static_cast<IndevType>(lv_indev_get_type(indev_))
                : IndevType::None;
}

void InputDevice::reset(Object* obj) {
  if (indev_) lv_indev_reset(indev_, obj ? obj->raw() : nullptr);
}

void InputDevice::stop_processing() {
  if (indev_) lv_indev_stop_processing(indev_);
}

void InputDevice::enable(bool en) {
  if (indev_) lv_indev_enable(indev_, en);
}

void InputDevice::read() {
  if (indev_) lv_indev_read(indev_);
}

IndevState InputDevice::get_state() const {
  return indev_ ? static_cast<IndevState>(lv_indev_get_state(indev_))
                : IndevState::Released;
}

void InputDevice::get_point(lv_point_t* point) {
  if (indev_) lv_indev_get_point(indev_, point);
}

lv_dir_t InputDevice::get_gesture_dir() {
  return indev_ ? lv_indev_get_gesture_dir(indev_) : LV_DIR_NONE;
}

uint32_t InputDevice::get_key() {
  return indev_ ? lv_indev_get_key(indev_) : 0;
}

lv_dir_t InputDevice::get_scroll_dir() {
  return indev_ ? lv_indev_get_scroll_dir(indev_) : LV_DIR_NONE;
}

lv_obj_t* InputDevice::get_scroll_obj() {
  return indev_ ? lv_indev_get_scroll_obj(indev_) : nullptr;
}

}  // namespace lvgl
