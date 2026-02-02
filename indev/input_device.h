#ifndef LVGL_CPP_INDEV_INPUT_DEVICE_H_
#define LVGL_CPP_INDEV_INPUT_DEVICE_H_

#include <functional>
#include <memory>
#include <vector>

#include "../core/object.h"
#include "../misc/enums.h"
#include "../misc/geometry.h"
#include "gesture_proxy.h"
#include "lvgl.h"

/**
 * @file input_device.h
 * @brief User Guide:
 * `InputDevice` wraps `lv_indev_t`, managing touchscreens, mice, keyboards, and
 * encoders.
 *
 * Key Features:
 * - **Abstraction**: Common API for all input device types.
 * - **Read Callbacks**: Easy integration with hardware via `set_read_cb`.
 * - **Gestures**: Proxy-based access to gesture state.
 * - **Type Safety**: Uses `IndevType` and `IndevState` enums.
 */

namespace lvgl {

class IndevData;

/**
 * @brief Wrapper for LVGL input devices.
 */
class InputDevice {
 public:
  /** @brief Input device type alias. */
  using Type = IndevType;
  /** @brief Input device state alias. */
  using State = IndevState;

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
  static InputDevice create(Type type);
  static InputDevice wrap(lv_indev_t* indev) {
    return InputDevice(indev, Object::Ownership::Unmanaged);
  }

  /**
   * @brief Access to active device.
   * @return Pointer to the active input device.
   */
  static InputDevice* get_active();

  /**
   * @brief Get the next input device in the list.
   * @param indev Current device.
   * @return Pointer to the next device.
   */
  static InputDevice* get_next(InputDevice* indev);

  // Configuration

  /**
   * @brief Set the read callback for the input device.
   * @param cb Callback function that populates IndevData.
   */
  void set_read_cb(std::function<void(IndevData&)> cb);

  /**
   * @brief Set the input device type.
   * @param type The type (pointer, keypad, button, encoder).
   */
  void set_type(Type type);

  /**
   * @brief Set the input device mode.
   * @param mode Timer or Event based reading.
   */
  void set_mode(lv_indev_mode_t mode);

  /**
   * @brief Set a cursor object (for pointer devices).
   * @param cursor The UI object to use as a cursor.
   */
  void set_cursor(Object& cursor);

  /**
   * @brief Set time to trigger a long press.
   * @param time Time in ms.
   */
  void set_long_press_time(uint16_t time);

  /**
   * @brief Set time to repeat a long press.
   * @param time Time in ms.
   */
  void set_long_press_repeat_time(uint16_t time);

  /**
   * @brief Set scroll limit in pixels.
   * @param limit Limit in pixels.
   */
  void set_scroll_limit(uint8_t limit);

  /**
   * @brief Set scroll throw decay percentage.
   * @param throw_decay Decay rate.
   */
  void set_scroll_throw(uint8_t throw_decay);

  /**
   * @brief Assign the device to a specific display.
   * @param disp Pointer to the display.
   */
  void set_display(lv_display_t* disp);

  /**
   * @brief Set driver-specific user data.
   * @param data Pointer to user data.
   */
  void set_driver_data(void* data);

  /**
   * @brief Assign physical points to virtual buttons.
   * @param points Array of points.
   */
  void set_button_points(const lv_point_t points[]);

  // Events
  /**
   * @brief Add an event callback to the device.
   * @param cb The callback function.
   * @param filter Event code to filter for.
   */
  void add_event_cb(std::function<void(lv_event_t*)> cb,
                    EventCode filter = EventCode::All);

  /**
   * @brief Send a custom event to the device.
   * @param code Event code.
   * @param param Event parameters.
   */
  void send_event(EventCode code, void* param);

  /**
   * @brief Remove an event callback by index.
   * @param index Callback index.
   * @return true if removed.
   */
  bool remove_event(uint32_t index);

  /**
   * @brief Remove event callbacks matching a function and data.
   */
  void remove_event_cb_with_user_data(lv_event_cb_t cb, void* user_data);

  /** @brief Get the number of registered event callbacks. */
  uint32_t get_event_count() const;

  /** @brief Get event descriptor by index. */
  lv_event_dsc_t* get_event_dsc(uint32_t index) const;

  // Methods
  /** @brief Get the device type. */
  Type get_type() const;

  /** @brief Reset the device state, optionally targeting an object. */
  void reset(Object* obj);

  /** @brief Reset long press tracking. */
  void reset_long_press();

  /** @brief Stop processing current input. */
  void stop_processing();

  /** @brief Enable or disable the device. */
  void enable(bool en);

  /** @brief Wait for the input to be released. */
  void wait_release();

  /** @brief Manually trigger a read operation. */
  void read();

  /** @brief Provides a proxy for gesture-related state. */
  GestureProxy gestures() { return GestureProxy(this); }

  /** @brief Get the reading mode. */
  lv_indev_mode_t get_mode() const;

  // Data Access
  /** @brief Get current state (Pressed/Released). */
  State get_state() const;

  /** @brief Get current coordinate (for pointers). */
  void get_point(lv_point_t* point);

  /** @brief Get vector of last movement. */
  Point get_vect() const;

  /** @brief Get current gesture direction. */
  lv_dir_t get_gesture_dir();

  /** @brief Get current key code (for keypads). */
  uint32_t get_key();

  /** @brief Get count of short clicks in a row. */
  uint8_t get_short_click_streak() const;

  /** @brief Get scrolling direction. */
  lv_dir_t get_scroll_dir();

  /** @brief Get current object being scrolled. */
  lv_obj_t* get_scroll_obj();

  /** @brief Get the internal read timer. */
  lv_timer_t* get_read_timer();

  /** @brief Get associated display. */
  lv_display_t* get_display();

  /** @brief Get associated input group. */
  lv_group_t* get_group() const;

  /** @brief Set associated input group. */
  void set_group(lv_group_t* group);

  /** @brief Set associated input group. */
  void set_group(Group& group);

  /** @brief Get currently pressed object. */
  lv_obj_t* get_active_obj() const;

  /** @brief Get cursor object. */
  lv_obj_t* get_cursor() const;

  /** @brief Get driver data. */
  void* get_driver_data() const;

  /** @brief Check if the press has moved (outside drag limit). */
  bool get_press_moved() const;

  /** @brief Search for an object at a coordinate. */
  lv_obj_t* search_obj(const Point& p) const;

  /** @brief Get underlying LVGL handle. */
  lv_indev_t* raw() const { return indev_; }

  // Internal callback dispatcher
  void process_read(lv_indev_data_t* data);

 private:
  lv_indev_t* indev_ = nullptr;
  bool owned_ = false;
  std::function<void(IndevData&)> read_cb_;
  std::vector<std::unique_ptr<EventCallbackData>> event_callbacks_;
};

}  // namespace lvgl

#include "indev_data.h"

#endif  // LVGL_CPP_INDEV_INPUT_DEVICE_H_
