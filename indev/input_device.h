#ifndef LVGL_CPP_INDEV_INPUT_DEVICE_H_
#define LVGL_CPP_INDEV_INPUT_DEVICE_H_

#include <cstdint>
#include <functional>
#include <memory>
#include <vector>

#include "../core/object.h"  // IWYU pragma: export
#include "../display/display.h"
#include "../misc/geometry.h"
#include "gesture_proxy.h"
#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

class IndevData;  // Forward declaration

class InputDevice {
 public:
  /**
   * @brief Input device types.
   */
  enum class Type : uint8_t {
    None = LV_INDEV_TYPE_NONE,
    Pointer = LV_INDEV_TYPE_POINTER,
    Keypad = LV_INDEV_TYPE_KEYPAD,
    Button = LV_INDEV_TYPE_BUTTON,
    Encoder = LV_INDEV_TYPE_ENCODER,
  };

  /**
   * @brief Input device states.
   */
  enum class State : uint8_t {
    Released = LV_INDEV_STATE_RELEASED,
    Pressed = LV_INDEV_STATE_PRESSED,
  };

  // Constructors & Destructor
  struct EventCallbackData {
    std::function<void(lv_event_t*)> cb;
    InputDevice* instance;
  };

  InputDevice();  // Default constructor (inactive)
  explicit InputDevice(lv_indev_t* indev, Object::Ownership ownership =
                                              Object::Ownership::Default);
  virtual ~InputDevice();

  // Move-only semantics
  InputDevice(const InputDevice&) = delete;
  InputDevice& operator=(const InputDevice&) = delete;
  InputDevice(InputDevice&& other) noexcept;
  InputDevice& operator=(InputDevice&& other) noexcept;

  // Factory
  static InputDevice create(lv_indev_type_t type);
  static InputDevice create(Type type) {
    return create(static_cast<lv_indev_type_t>(type));
  }
  static InputDevice wrap(lv_indev_t* indev) {
    return InputDevice(indev, Object::Ownership::Unmanaged);
  }

  // Access to active device
  static InputDevice* get_active();
  static InputDevice* get_next(InputDevice* indev);

  // Configuration
  [[deprecated("Use set_read_cb(std::function<void(IndevData&)>) instead.")]]
  void set_read_cb(std::function<void(lv_indev_data_t*)> cb);
  void set_read_cb(std::function<void(IndevData&)> cb);
  void set_type(lv_indev_type_t type);
  void set_mode(lv_indev_mode_t mode);
  void set_cursor(Object& cursor);
  void set_long_press_time(uint16_t time);
  void set_long_press_repeat_time(uint16_t time);
  void set_scroll_limit(uint8_t limit);
  void set_scroll_throw(uint8_t throw_decay);
  void set_display(lv_display_t* disp);
  void set_driver_data(void* data);
  void set_button_points(const lv_point_t points[]);

  // Events
  void add_event_cb(std::function<void(lv_event_t*)> cb,
                    lv_event_code_t filter = LV_EVENT_ALL);
  void send_event(lv_event_code_t code, void* param);
  bool remove_event(uint32_t index);
  void remove_event_cb_with_user_data(lv_event_cb_t cb, void* user_data);
  uint32_t get_event_count() const;
  lv_event_dsc_t* get_event_dsc(uint32_t index) const;

  // Methods
  Type get_type() const;
  void reset(Object* obj);
  void reset_long_press();
  void stop_processing();
  void enable(bool en);
  void wait_release();
  void read();  // Manual trigger

  GestureProxy gestures() { return GestureProxy(this); }

  lv_indev_mode_t get_mode() const;

  // Data Access
  State get_state() const;
  void get_point(lv_point_t* point);
  Point get_vect() const;
  lv_dir_t get_gesture_dir();
  uint32_t get_key();
  uint8_t get_short_click_streak() const;
  lv_dir_t get_scroll_dir();
  lv_obj_t* get_scroll_obj();
  lv_timer_t* get_read_timer();
  lv_display_t* get_display();
  lv_group_t* get_group() const;
  void set_group(lv_group_t* group);
  void set_group(Group& group);
  lv_obj_t* get_active_obj() const;
  lv_obj_t* get_cursor() const;
  void* get_driver_data() const;
  bool get_press_moved() const;
  lv_obj_t* search_obj(const Point& p) const;

  lv_indev_t* raw() const { return indev_; }

  // Internal callback dispatcher
  void process_read(lv_indev_data_t* data);

 private:
  lv_indev_t* indev_ = nullptr;
  bool owned_ = false;
  std::function<void(lv_indev_data_t*)> read_cb_raw_;
  std::function<void(IndevData&)> read_cb_;
  std::vector<std::unique_ptr<EventCallbackData>> event_callbacks_;
};

}  // namespace lvgl

#include "indev_data.h"

#endif  // LVGL_CPP_INDEV_INPUT_DEVICE_H_
