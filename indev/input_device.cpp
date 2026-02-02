#include "input_device.h"

#include "../core/group.h"
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

InputDevice InputDevice::create(Type type) {
  lv_indev_t* indev = lv_indev_create();
  lv_indev_set_type(indev, static_cast<lv_indev_type_t>(type));
  return InputDevice(indev, Object::Ownership::Managed);
}

InputDevice* InputDevice::get_active() {
  static InputDevice instance(nullptr, Object::Ownership::Unmanaged);
  lv_indev_t* act = lv_indev_active();
  if (!act) return nullptr;
  instance = InputDevice(act, Object::Ownership::Unmanaged);
  return &instance;
}

InputDevice* InputDevice::get_next(InputDevice* indev) {
  static InputDevice instance(nullptr, Object::Ownership::Unmanaged);
  lv_indev_t* next = lv_indev_get_next(indev ? indev->raw() : nullptr);
  if (!next) return nullptr;
  instance = InputDevice(next, Object::Ownership::Unmanaged);
  return &instance;
}

void InputDevice::set_read_cb(std::function<void(IndevData&)> cb) {
  read_cb_ = cb;
  if (indev_) {
    lv_indev_set_user_data(indev_, this);
    lv_indev_set_read_cb(indev_, cpp_read_cb_trampoline);
  }
}

void InputDevice::process_read(lv_indev_data_t* data) {
  if (read_cb_) {
    IndevData wrapped(data);
    read_cb_(wrapped);
  }
}

void InputDevice::set_type(Type type) {
  if (indev_) lv_indev_set_type(indev_, static_cast<lv_indev_type_t>(type));
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

void InputDevice::set_scroll_throw(uint8_t throw_decay) {
  if (indev_) lv_indev_set_scroll_throw(indev_, throw_decay);
}

void InputDevice::set_display(lv_display_t* disp) {
  if (indev_) lv_indev_set_display(indev_, disp);
}

void InputDevice::set_driver_data(void* data) {
  if (indev_) lv_indev_set_driver_data(indev_, data);
}

void InputDevice::set_button_points(const lv_point_t points[]) {
  if (indev_) lv_indev_set_button_points(indev_, points);
}

void InputDevice::add_event_cb(std::function<void(lv_event_t*)> cb,
                               EventCode filter) {
  if (!indev_) return;
  auto data = std::make_unique<EventCallbackData>();
  data->cb = cb;
  data->instance = this;
  lv_indev_add_event_cb(indev_, indev_event_cb_proxy,
                        static_cast<lv_event_code_t>(filter), data.get());
  event_callbacks_.push_back(std::move(data));
}

void InputDevice::send_event(EventCode code, void* param) {
  if (indev_)
    lv_indev_send_event(indev_, static_cast<lv_event_code_t>(code), param);
}

bool InputDevice::remove_event(uint32_t index) {
  return indev_ ? lv_indev_remove_event(indev_, index) : false;
}

void InputDevice::remove_event_cb_with_user_data(lv_event_cb_t cb,
                                                 void* user_data) {
  if (indev_) lv_indev_remove_event_cb_with_user_data(indev_, cb, user_data);
}

uint32_t InputDevice::get_event_count() const {
  return indev_ ? lv_indev_get_event_count(indev_) : 0;
}

lv_event_dsc_t* InputDevice::get_event_dsc(uint32_t index) const {
  return indev_ ? lv_indev_get_event_dsc(indev_, index) : nullptr;
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

lv_group_t* InputDevice::get_group() const {
  return indev_ ? lv_indev_get_group(indev_) : nullptr;
}

void InputDevice::set_group(lv_group_t* group) {
  if (indev_) lv_indev_set_group(indev_, group);
}

void InputDevice::set_group(Group& group) {
  if (indev_) lv_indev_set_group(indev_, group.raw());
}

lv_obj_t* InputDevice::get_active_obj() const {
  return lv_indev_get_active_obj();
}

lv_obj_t* InputDevice::get_cursor() const {
  return indev_ ? lv_indev_get_cursor(indev_) : nullptr;
}

void* InputDevice::get_driver_data() const {
  return indev_ ? lv_indev_get_driver_data(indev_) : nullptr;
}

bool InputDevice::get_press_moved() const {
  return indev_ ? lv_indev_get_press_moved(indev_) : false;
}

lv_obj_t* InputDevice::search_obj(const Point& p) const {
  if (!indev_) return nullptr;
  lv_display_t* disp = lv_indev_get_display(indev_);
  lv_obj_t* scr = lv_display_get_screen_active(disp);
  lv_point_t pt = {static_cast<lv_coord_t>(p.x()),
                   static_cast<lv_coord_t>(p.y())};
  return lv_indev_search_obj(scr, &pt);
}

// --- GestureProxy ---

GestureProxy& GestureProxy::on_gesture(std::function<void(GestureEvent)> cb) {
  if (indev_) {
    indev_->add_event_cb(
        [cb](lv_event_t* e) {
          GestureEvent ge(e);
          cb(ge);
        },
        EventCode::Gesture);
  }
  return *this;
}

}  // namespace lvgl
