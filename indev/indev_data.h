#ifndef LVGL_CPP_INDEV_INDEV_DATA_H_
#define LVGL_CPP_INDEV_INDEV_DATA_H_

#include "../misc/enums.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Wrapper for lv_indev_data_t to provide type-safe accessors.
 */
class IndevData {
 public:
  explicit IndevData(lv_indev_data_t* data) : data_(data) {}

  /**
   * @brief Set the input device state.
   */
  void set_state(IndevState state) {
    data_->state = static_cast<lv_indev_state_t>(state);
  }

  /**
   * @brief Get the input device state.
   */
  IndevState get_state() const { return static_cast<IndevState>(data_->state); }

  /**
   * @brief Set the point for pointer input devices.
   */
  void set_point(int32_t x, int32_t y) {
    data_->point.x = x;
    data_->point.y = y;
  }

  /**
   * @brief Set the point for pointer input devices.
   */
  void set_point(lv_point_t point) { data_->point = point; }

  /**
   * @brief Get the point for pointer input devices.
   */
  lv_point_t get_point() const { return data_->point; }

  /**
   * @brief Set the key for keypad input devices.
   */
  void set_key(uint32_t key) { data_->key = key; }

  /**
   * @brief Set the key for keypad input devices using lvgl::Key enum.
   */
  void set_key(Key key) { data_->key = static_cast<uint32_t>(key); }

  /**
   * @brief Get the key for keypad input devices.
   */
  uint32_t get_key() const { return data_->key; }

  /**
   * @brief Set the button ID for button input devices.
   */
  void set_btn_id(uint32_t btn_id) { data_->btn_id = btn_id; }

  /**
   * @brief Get the button ID for button input devices.
   */
  uint32_t get_btn_id() const { return data_->btn_id; }

  /**
   * @brief Set the encoder difference for encoder input devices.
   */
  void set_enc_diff(int16_t enc_diff) { data_->enc_diff = enc_diff; }

  /**
   * @brief Get the encoder difference for encoder input devices.
   */
  int16_t get_enc_diff() const { return data_->enc_diff; }

  /**
   * @brief Set whether the input device should be read again.
   */
  void set_continue_reading(bool continue_reading) {
    data_->continue_reading = continue_reading;
  }

  /**
   * @brief Get whether the input device should be read again.
   */
  bool continue_reading() const { return data_->continue_reading; }

  /**
   * @brief Get the raw LVGL indev data pointer.
   */
  lv_indev_data_t* raw() const { return data_; }

 private:
  lv_indev_data_t* data_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_INDEV_INDEV_DATA_H_
