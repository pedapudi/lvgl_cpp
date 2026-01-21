#include "gpio_input.h"

#include "esp_log.h"
#include "lvgl_cpp/misc/enums.h"

static const char* TAG = "GpioInput";

GpioInput::GpioInput(const Config& config) : config_(config) {
  // Initialize GPIOs
  gpio_config_t io_conf = {.pin_bit_mask = (1ULL << config_.next_pin) | (1ULL << config_.enter_pin),
                           .mode = GPIO_MODE_INPUT,
                           .pull_up_en = GPIO_PULLUP_ENABLE,
                           .pull_down_en = GPIO_PULLDOWN_DISABLE,
                           .intr_type = GPIO_INTR_DISABLE};
  gpio_config(&io_conf);

  // Create Keypad Input
  input_ = lvgl::KeypadInput::create();

  // Set the read callback
  input_.set_read_cb([this](lv_indev_data_t* data) { this->read_cb(data); });
}

void GpioInput::read_cb(lv_indev_data_t* data) {
  // Read the physical state of the GPIO pins (active low buttons).
  bool next_pressed = !gpio_get_level(config_.next_pin);
  bool enter_pressed = !gpio_get_level(config_.enter_pin);

  // Logic: Map the physical buttons to LVGL virtual keys.

  // CRITICAL FOR BEGINNERS:
  // LVGL has two types of "next" navigation:
  // 1. LV_KEY_NEXT: Moves focus between different widgets in a group.
  // 2. LV_KEY_DOWN/UP: Navigates inside a widget (like scrolling a list or roller).
  //
  // Because our application uses a Roller which handles UP/DOWN natively for
  // scrolling, we map our "Next" button to LV_KEY_DOWN.
  if (next_pressed) {
    ESP_LOGI(TAG, "DOWN key pressed (for roller scroll)");
    data->key = static_cast<uint32_t>(lvgl::Key::Down);
    data->state = static_cast<lv_indev_state_t>(lvgl::IndevState::Pressed);
    last_key_ = static_cast<uint32_t>(lvgl::Key::Down);
  } else if (enter_pressed) {
    // LV_KEY_ENTER is used to select the currently focused item.
    ESP_LOGI(TAG, "ENTER key pressed");
    data->key = static_cast<uint32_t>(lvgl::Key::Enter);
    data->state = static_cast<lv_indev_state_t>(lvgl::IndevState::Pressed);
    last_key_ = static_cast<uint32_t>(lvgl::Key::Enter);
  } else {
    // When no button is pressed, we report the last key as "RELEASED".
    data->key = last_key_;
    data->state = static_cast<lv_indev_state_t>(lvgl::IndevState::Released);
  }
}
