#ifndef LVGL_CPP_INDEV_INDEV_DATA_H_
#define LVGL_CPP_INDEV_INDEV_DATA_H_

#include "../misc/enums.h"  // Keep this for InputDevice::State
#include "lvgl.h"

namespace lvgl {

class InputDevice;  // Forward declaration for InputDevice class

/**
 * @brief Wrapper for lv_indev_data_t to provide type-safe accessors.
 *
 * Provides a fluent API for setting input device data, consistent with
 * the rest of lvgl_cpp.
 */
class IndevData {
 public:
  explicit IndevData(lv_indev_data_t* data) : data_(data) {}

  /**
   * @brief Set the input device state.
   */
  IndevData& set_state(InputDevice::State state) {
    data_->state = static_cast<lv_indev_state_t>(state);
    return *this;
  }

  /**
   * @brief Get the input device state.
   */
  InputDevice::State get_state() const {
    return static_cast<InputDevice::State>(data_->state);
  }

  /**
   * @brief Set the point for pointer input devices.
   */
  IndevData& set_point(int32_t x, int32_t y) {
    data_->point.x = x;
    data_->point.y = y;
    return *this;
  }

  /**
   * @brief Set the point for pointer input devices.
   */
  IndevData& set_point(lv_point_t point) {
    data_->point = point;
    return *this;
  }

  /**
   * @brief Get the point for pointer input devices.
   */
  lv_point_t get_point() const { return data_->point; }

  /**
   * @brief Set the key for keypad input devices using lvgl::Key enum.
   */
  IndevData& set_key(Key key) {
    data_->key = static_cast<uint32_t>(key);
    return *this;
  }

  /**
   * @brief Get the key for keypad input devices.
   */
  Key get_key() const { return static_cast<Key>(data_->key); }

  /**
   * @brief Set the button ID for button input devices.
   */
  IndevData& set_btn_id(uint32_t btn_id) {
    data_->btn_id = btn_id;
    return *this;
  }

  /**
   * @brief Get the button ID for button input devices.
   */
  uint32_t get_btn_id() const { return data_->btn_id; }

  /**
   * @brief Set the encoder difference for encoder input devices.
   */
  IndevData& set_enc_diff(int16_t enc_diff) {
    data_->enc_diff = enc_diff;
    return *this;
  }

  /**
   * @brief Get the encoder difference for encoder input devices.
   */
  int16_t get_enc_diff() const { return data_->enc_diff; }

  /**
   * @brief Set whether the input device should be read again.
   */
  IndevData& set_continue_reading(bool continue_reading) {
    data_->continue_reading = continue_reading;
    return *this;
  }

  /**
   * @brief Get whether the input device should be read again.
   */
  bool continue_reading() const { return data_->continue_reading; }

  /**
   * @brief Set the timestamp of the event.
   */
  IndevData& set_timestamp(uint32_t timestamp) {
    data_->timestamp = timestamp;
    return *this;
  }

  /**
   * @brief Get the timestamp of the event.
   */
  uint32_t get_timestamp() const { return data_->timestamp; }

  /**
   * @brief Set gesture type for a specific gesture index.
   */
  IndevData& set_gesture_type(lv_indev_gesture_type_t gesture,
                              lv_indev_gesture_type_t type) {
    if (gesture < LV_INDEV_GESTURE_CNT) {
      data_->gesture_type[gesture] = type;
    }
    return *this;
  }

  /**
   * @brief Get gesture type for a specific gesture index.
   */
  lv_indev_gesture_type_t get_gesture_type(
      lv_indev_gesture_type_t gesture) const {
    return (gesture < LV_INDEV_GESTURE_CNT) ? data_->gesture_type[gesture]
                                            : LV_INDEV_GESTURE_NONE;
  }

  /**
   * @brief Set gesture data for a specific gesture index.
   */
  IndevData& set_gesture_data(lv_indev_gesture_type_t gesture, void* data) {
    if (gesture < LV_INDEV_GESTURE_CNT) {
      data_->gesture_data[gesture] = data;
    }
    return *this;
  }

  /**
   * @brief Get gesture data for a specific gesture index.
   */
  void* get_gesture_data(lv_indev_gesture_type_t gesture) const {
    return (gesture < LV_INDEV_GESTURE_CNT) ? data_->gesture_data[gesture]
                                            : nullptr;
  }

  /**
   * @brief Get the raw LVGL indev data pointer.
   */
  lv_indev_data_t* raw() const { return data_; }

 private:
  lv_indev_data_t* data_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_INDEV_DATA_H_
