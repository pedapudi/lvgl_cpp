#ifndef LVGL_CPP_INDEV_INPUT_DEVICE_H_
#define LVGL_CPP_INDEV_INPUT_DEVICE_H_

#include <cstdint>
#include <functional>

#include "indev_data.h"
// #include "../core/group.h"   // Removed: cleanup
#include "../core/object.h"  // IWYU pragma: export
#include "../misc/enums.h"   // For IndevType, IndevState
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
  [[deprecated("Use set_read_cb(std::function<void(IndevData&)>) instead.")]]
  void set_read_cb(std::function<void(lv_indev_data_t*)> cb);
  void set_read_cb(std::function<void(IndevData&)> cb);
  void set_type(lv_indev_type_t type);
  void set_cursor(Object& cursor);

  // Methods
  IndevType get_type() const;
  void reset(Object* obj);
  void stop_processing();
  void enable(bool en);
  void read();  // Manual trigger

  // Data Access
  IndevState get_state() const;
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
  std::function<void(lv_indev_data_t*)> read_cb_raw_;
  std::function<void(IndevData&)> read_cb_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_INPUT_DEVICE_H_
