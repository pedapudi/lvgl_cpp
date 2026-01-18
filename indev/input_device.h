#ifndef LVGL_CPP_INDEV_INPUT_DEVICE_H_
#define LVGL_CPP_INDEV_INPUT_DEVICE_H_

#include <cstdint>
#include <functional>

#include "../core/group.h"   // IWYU pragma: export
#include "../core/object.h"  // IWYU pragma: export
#include "lvgl.h"            // IWYU pragma: export

namespace lvgl {

class InputDevice {
 public:
  // Constructors & Destructor
  InputDevice();  // Default constructor (inactive)
  explicit InputDevice(lv_indev_t* indev, bool owned = false);
  virtual ~InputDevice();

  // Move-only semantics
  InputDevice(const InputDevice&) = delete;
  InputDevice& operator=(const InputDevice&) = delete;
  InputDevice(InputDevice&& other) noexcept;
  InputDevice& operator=(InputDevice&& other) noexcept;

  // Factory
  static InputDevice create(lv_indev_type_t type);
  static InputDevice wrap(lv_indev_t* indev) {
    return InputDevice(indev, false);
  }

  // Access to active device
  static InputDevice* get_act();

  // Configuration
  void set_read_cb(std::function<void(lv_indev_data_t*)> cb);
  void set_type(lv_indev_type_t type);

  // Methods
  lv_indev_type_t get_type();
  void reset(Object* obj);
  void stop_processing();
  void set_group(Group* group);
  void set_cursor(Object* cur_obj);
  void enable(bool en);
  void read();  // Manual trigger

  // Data Access
  lv_indev_state_t get_state();
  void get_point(lv_point_t* point);
  lv_dir_t get_gesture_dir();
  uint32_t get_key();
  lv_dir_t get_scroll_dir();
  lv_obj_t* get_scroll_obj();

  lv_indev_t* raw() const { return indev_; }

  // Internal callback dispatcher
  void process_read(lv_indev_data_t* data);

 private:
  lv_indev_t* indev_ = nullptr;
  bool owned_ = false;
  std::function<void(lv_indev_data_t*)> read_cb_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_INPUT_DEVICE_H_
