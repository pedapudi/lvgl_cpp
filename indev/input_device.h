#ifndef LVGL_CPP_INDEV_INPUT_DEVICE_H_
#define LVGL_CPP_INDEV_INPUT_DEVICE_H_

#include "../core/group.h"   // IWYU pragma: export
#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

namespace lvgl {

class InputDevice {
 public:
  explicit InputDevice(lv_indev_t* indev);

  static InputDevice* get_act();

  lv_indev_type_t get_type();
  void reset(Object* obj);
  void stop_processing();
  void set_group(Group* group);
  void set_cursor(Object* cur_obj);

  lv_indev_state_t get_state();
  void get_point(lv_point_t* point);
  lv_dir_t get_gesture_dir();
  uint32_t get_key();
  lv_dir_t get_scroll_dir();
  lv_obj_t* get_scroll_obj();

  lv_indev_t* raw() const { return indev_; }

 private:
  lv_indev_t* indev_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_INPUT_DEVICE_H_
