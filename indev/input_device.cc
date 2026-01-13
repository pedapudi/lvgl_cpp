#include "input_device.h"

namespace lvgl {

InputDevice::InputDevice(lv_indev_t* indev) : indev_(indev) {}

InputDevice* InputDevice::get_act() {
  static InputDevice instance(nullptr);
  instance = InputDevice(lv_indev_active());
  return &instance;
}

lv_indev_type_t InputDevice::get_type() {
  return indev_ ? lv_indev_get_type(indev_) : LV_INDEV_TYPE_NONE;
}

void InputDevice::reset(Object* obj) {
  if (indev_) lv_indev_reset(indev_, obj ? obj->raw() : nullptr);
}

void InputDevice::stop_processing() {
  if (indev_) lv_indev_stop_processing(indev_);
}

void InputDevice::set_group(Group* group) {
  if (indev_ && group) lv_indev_set_group(indev_, group->raw());
}

void InputDevice::set_cursor(Object* cur_obj) {
  if (indev_ && cur_obj) lv_indev_set_cursor(indev_, cur_obj->raw());
}

lv_indev_state_t InputDevice::get_state() {
  return indev_ ? lv_indev_get_state(indev_) : LV_INDEV_STATE_RELEASED;
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
