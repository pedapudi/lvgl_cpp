#include "input_device.h"

#include "gesture_event.h"
#include "gesture_proxy.h"

namespace lvgl {

// Static trampoline to bridge C -> C++
static void cpp_read_cb_trampoline(lv_indev_t* indev, lv_indev_data_t* data) {
  auto* instance = static_cast<InputDevice*>(lv_indev_get_user_data(indev));
  if (instance) {
    instance->process_read(data);
  }
}

static void indev_event_cb_proxy(lv_event_t* e) {
  auto* data =
      static_cast<InputDevice::EventCallbackData*>(lv_event_get_user_data(e));
  if (data && data->cb) {
    data->cb(e);
  }
}

InputDevice::InputDevice() : indev_(nullptr), owned_(false) {}

InputDevice::InputDevice(lv_indev_t* indev, Object::Ownership ownership)
    : indev_(indev), owned_(ownership == Object::Ownership::Managed) {}

InputDevice::~InputDevice() {
  if (owned_ && indev_) {
    lv_indev_delete(indev_);
  }
}

InputDevice::InputDevice(InputDevice&& other) noexcept
    : indev_(other.indev_),
      owned_(other.owned_),
      read_cb_raw_(std::move(other.read_cb_raw_)),
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
    read_cb_raw_ = std::move(other.read_cb_raw_);
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
  return InputDevice(indev, Object::Ownership::Managed);
}

InputDevice* InputDevice::get_act() {
  static InputDevice instance(nullptr, Object::Ownership::Unmanaged);
  // This is tricky: get_act returns a wrapper around the active indev.
  // It is NOT the original C++ object if one exists.
  // We can check user_data, but for safety, we return a temporary-ish wrapper.
  // Ideally, this returns a pointer to the singleton wrapper.
  // For now, let's keep the existing pattern but be aware it's unowned.
  lv_indev_t* act = lv_indev_active();
  if (!act) return nullptr;

  // Re-use the static instance to wrap the current active device
  instance = InputDevice(act, Object::Ownership::Unmanaged);
  return &instance;
}

void InputDevice::set_read_cb(std::function<void(lv_indev_data_t*)> cb) {
  read_cb_raw_ = cb;
  read_cb_ = nullptr;
  if (indev_) {
    lv_indev_set_user_data(indev_, this);
    lv_indev_set_read_cb(indev_, cpp_read_cb_trampoline);
  }
}

void InputDevice::set_read_cb(std::function<void(IndevData&)> cb) {
  read_cb_ = cb;
  read_cb_raw_ = nullptr;
  if (indev_) {
    lv_indev_set_user_data(indev_, this);
    lv_indev_set_read_cb(indev_, cpp_read_cb_trampoline);
  }
}

void InputDevice::process_read(lv_indev_data_t* data) {
  if (read_cb_) {
    IndevData wrapped(data);
    read_cb_(wrapped);
  } else if (read_cb_raw_) {
    read_cb_raw_(data);
  }
}

void InputDevice::set_type(lv_indev_type_t type) {
  if (indev_) lv_indev_set_type(indev_, type);
}

void InputDevice::set_cursor(Object& cursor) {
  if (indev_) lv_indev_set_cursor(indev_, cursor.raw());
}

void InputDevice::set_mode(lv_indev_mode_t mode) {
  if (indev_) lv_indev_set_mode(indev_, mode);
}

void InputDevice::set_long_press_time(uint16_t time) {
  if (indev_) lv_indev_set_long_press_time(indev_, time);
}

void InputDevice::set_long_press_repeat_time(uint16_t time) {
  if (indev_) lv_indev_set_long_press_repeat_time(indev_, time);
}

void InputDevice::set_scroll_limit(uint8_t limit) {
  if (indev_) lv_indev_set_scroll_limit(indev_, limit);
}

void InputDevice::add_event_cb(std::function<void(lv_event_t*)> cb,
                               lv_event_code_t filter) {
  if (!indev_) return;
  auto data = std::make_unique<EventCallbackData>();
  data->cb = cb;
  data->instance = this;
  lv_indev_add_event_cb(indev_, indev_event_cb_proxy, filter, data.get());
  event_callbacks_.push_back(std::move(data));
}

// ... wrapped methods ...

InputDevice::Type InputDevice::get_type() const {
  return indev_ ? static_cast<Type>(lv_indev_get_type(indev_)) : Type::None;
}

void InputDevice::reset(Object* obj) {
  if (indev_) lv_indev_reset(indev_, obj ? obj->raw() : nullptr);
}

void InputDevice::reset_long_press() {
  if (indev_) lv_indev_reset_long_press(indev_);
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

void InputDevice::wait_release() {
  if (indev_) lv_indev_wait_release(indev_);
}

lv_indev_mode_t InputDevice::get_mode() const {
  return indev_ ? lv_indev_get_mode(indev_) : LV_INDEV_MODE_NONE;
}

InputDevice::State InputDevice::get_state() const {
  return indev_ ? static_cast<State>(lv_indev_get_state(indev_))
                : State::Released;
}

void InputDevice::get_point(lv_point_t* point) {
  if (indev_) lv_indev_get_point(indev_, point);
}

Point InputDevice::get_vect() const {
  lv_point_t p = {0, 0};
  if (indev_) lv_indev_get_vect(indev_, &p);
  return Point(p);
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

uint8_t InputDevice::get_short_click_streak() const {
  return indev_ ? lv_indev_get_short_click_streak(indev_) : 0;
}

lv_timer_t* InputDevice::get_read_timer() {
  return indev_ ? lv_indev_get_read_timer(indev_) : nullptr;
}

lv_display_t* InputDevice::get_display() {
  return indev_ ? lv_indev_get_display(indev_) : nullptr;
}

// --- GestureProxy ---

GestureProxy& GestureProxy::on_gesture(std::function<void(GestureEvent)> cb) {
  if (indev_) {
    indev_->add_event_cb(
        [cb](lv_event_t* e) {
          GestureEvent ge(e);
          cb(ge);
        },
        LV_EVENT_GESTURE);
  }
  return *this;
}

}  // namespace lvgl
