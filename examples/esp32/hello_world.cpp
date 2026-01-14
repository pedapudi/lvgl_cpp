/*
 * SPDX-FileCopyrightText: 2021-2025 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 *
 * Example Description:
 * This example is implemented for the Seeed Studio XIAO ESP32C3 development
 * board with an attached SSD1306 OLED display.
 */

#include <stdio.h>
#include <sys/lock.h>
#include <sys/param.h>
#include <unistd.h>

#include "driver/i2c_master.h"
#include "esp_err.h"
#include "esp_lcd_panel_io.h"
#include "esp_lcd_panel_ops.h"
#include "esp_log.h"
#include "esp_timer.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "lvgl.h"
#include "lvgl_cpp.h"

#define CONFIG_EXAMPLE_LCD_CONTROLLER_SSD1306 1

#if CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
#include "esp_lcd_sh1107.h"
#else
#include "esp_lcd_panel_vendor.h"
#endif

static const char* TAG = "example";

#define I2C_BUS_PORT 0

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////// Please update the following configuration according to your
/// LCD spec //////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define EXAMPLE_LCD_PIXEL_CLOCK_HZ (200 * 1000)
#define EXAMPLE_PIN_NUM_SDA 6
#define EXAMPLE_PIN_NUM_SCL 7
#define EXAMPLE_PIN_NUM_RST -1
#define EXAMPLE_I2C_HW_ADDR 0x3C

// The pixel number in horizontal and vertical
#if CONFIG_EXAMPLE_LCD_CONTROLLER_SSD1306
#define EXAMPLE_LCD_H_RES 128
#define EXAMPLE_LCD_V_RES 64
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
#define EXAMPLE_LCD_H_RES 64
#define EXAMPLE_LCD_V_RES 128
#endif
// Bit number used to represent command and parameter
#define EXAMPLE_LCD_CMD_BITS 8
#define EXAMPLE_LCD_PARAM_BITS 8

#define EXAMPLE_LVGL_TICK_PERIOD_MS 5
#define EXAMPLE_LVGL_TASK_STACK_SIZE (4 * 1024)
#define EXAMPLE_LVGL_TASK_PRIORITY 2
#define EXAMPLE_LVGL_PALETTE_SIZE 8
#define EXAMPLE_LVGL_TASK_MAX_DELAY_MS 500
#define EXAMPLE_LVGL_TASK_MIN_DELAY_MS 1000 / CONFIG_FREERTOS_HZ

// To use LV_COLOR_FORMAT_I1, we need an extra buffer to hold the converted data
static uint8_t oled_buffer[EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES / 8];
// LVGL library is not thread-safe, this example will call LVGL APIs from
// different tasks, so use a mutex to protect it
static _lock_t lvgl_api_lock;

void example_lvgl_demo_ui(lvgl::Display& display) {
  lvgl::Object scr(display.get_screen_active());
  lvgl::Label label(&scr, "Hello, world!");
  label.set_long_mode(LV_LABEL_LONG_SCROLL_CIRCULAR)
      .set_width(display.get_horizontal_resolution())
      .align(LV_ALIGN_BOTTOM_MID, 0, 0);
  // Create an animation to ease the label in from top
  // storage on stack is fine because start() copies the config to LVGL internal
  // memory
  lvgl::Animation(label)
      //.set_var(label.raw()) // Implicitly set by constructor
      .set_values(-50, 0)
      .set_duration(2000)
      .set_exec_cb(lvgl::Animation::Exec::Y())
      .set_path_cb(lvgl::Animation::Path::EaseOut())
      .set_completed_cb([]() { ESP_LOGI("Anim", "Animation completed!"); })
      .start();

  label.release();  // Release ownership to LVGL so it persists after function
                    // return
}

static bool example_notify_lvgl_flush_ready(
    esp_lcd_panel_io_handle_t io_panel, esp_lcd_panel_io_event_data_t* edata,
    void* user_ctx) {
  lvgl::Display* display = static_cast<lvgl::Display*>(user_ctx);
  display->flush_ready();
  return false;
}

static void example_lvgl_flush_cb(esp_lcd_panel_handle_t panel_handle,
                                  lvgl::Display* display, const lv_area_t* area,
                                  uint8_t* px_map) {
  // This is necessary because LVGL reserves 2 x 4 bytes in the buffer, as these
  // are assumed to be used as a palette. Skip the palette here More information
  // about the monochrome, please refer to
  // https://docs.lvgl.io/9.2/porting/display.html#monochrome-displays
  px_map += EXAMPLE_LVGL_PALETTE_SIZE;

  // We use raw() here because lvgl::Display wrapper for
  // get_physical_horizontal_resolution might execute logic, but let's see if we
  // can use the wrapper method directly. The header showed
  // get_physical_horizontal_resolution() exists.
  uint16_t hor_res = display->get_physical_horizontal_resolution();

  int x1 = area->x1;
  int x2 = area->x2;
  int y1 = area->y1;
  int y2 = area->y2;

  for (int y = y1; y <= y2; y++) {
    for (int x = x1; x <= x2; x++) {
      /* The order of bits is MSB first
                  MSB           LSB
         bits      7 6 5 4 3 2 1 0
         pixels    0 1 2 3 4 5 6 7
                  Left         Right
      */
      bool chroma_color =
          (px_map[(hor_res >> 3) * y + (x >> 3)] & 1 << (7 - x % 8));

      /* Write to the buffer as required for the display.
       * It writes only 1-bit for monochrome displays mapped vertically.*/
      uint8_t* buf = oled_buffer + hor_res * (y >> 3) + (x);
      if (chroma_color) {
        (*buf) &= ~(1 << (y % 8));
      } else {
        (*buf) |= (1 << (y % 8));
      }
    }
  }
  // pass the draw buffer to the driver
  esp_lcd_panel_draw_bitmap(panel_handle, x1, y1, x2 + 1, y2 + 1, oled_buffer);
}

static void example_increase_lvgl_tick(void* arg) {
  /* Tell LVGL how many milliseconds has elapsed */
  lv_tick_inc(EXAMPLE_LVGL_TICK_PERIOD_MS);
}

static void example_lvgl_port_task(void* arg) {
  ESP_LOGI(TAG, "Starting LVGL task");
  uint32_t time_till_next_ms = 0;
  while (1) {
    _lock_acquire(&lvgl_api_lock);
    time_till_next_ms = lv_timer_handler();
    _lock_release(&lvgl_api_lock);
    // in case of triggering a task watch dog time out
    time_till_next_ms = MAX(time_till_next_ms, EXAMPLE_LVGL_TASK_MIN_DELAY_MS);
    // in case of lvgl display not ready yet
    time_till_next_ms = MIN(time_till_next_ms, EXAMPLE_LVGL_TASK_MAX_DELAY_MS);
    usleep(1000 * time_till_next_ms);
  }
}

extern "C" void app_main(void) {
  ESP_LOGI(TAG, "Initialize I2C bus");
  i2c_master_bus_handle_t i2c_bus = NULL;
  i2c_master_bus_config_t bus_config = {};
  bus_config.clk_source = I2C_CLK_SRC_DEFAULT;
  bus_config.glitch_ignore_cnt = 7;
  bus_config.i2c_port = I2C_BUS_PORT;
  bus_config.sda_io_num = static_cast<gpio_num_t>(EXAMPLE_PIN_NUM_SDA);
  bus_config.scl_io_num = static_cast<gpio_num_t>(EXAMPLE_PIN_NUM_SCL);
  bus_config.flags.enable_internal_pullup = true;
  ESP_ERROR_CHECK(i2c_new_master_bus(&bus_config, &i2c_bus));

  ESP_LOGI(TAG, "Install panel IO");
  esp_lcd_panel_io_handle_t io_handle = NULL;
  esp_lcd_panel_io_i2c_config_t io_config = {};
  io_config.dev_addr = EXAMPLE_I2C_HW_ADDR;
  io_config.scl_speed_hz = EXAMPLE_LCD_PIXEL_CLOCK_HZ;
  io_config.control_phase_bytes = 1;
  io_config.lcd_cmd_bits = EXAMPLE_LCD_CMD_BITS;
  io_config.lcd_param_bits = EXAMPLE_LCD_CMD_BITS;
#if CONFIG_EXAMPLE_LCD_CONTROLLER_SSD1306
  io_config.dc_bit_offset = 6;
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
  io_config.dc_bit_offset = 0;
  io_config.flags.disable_control_phase = 1;
#endif
  ESP_ERROR_CHECK(esp_lcd_new_panel_io_i2c(i2c_bus, &io_config, &io_handle));

  ESP_LOGI(TAG, "Install SSD1306 panel driver");
  esp_lcd_panel_handle_t panel_handle = NULL;
  esp_lcd_panel_dev_config_t panel_config = {};
  panel_config.bits_per_pixel = 1;
  panel_config.reset_gpio_num = EXAMPLE_PIN_NUM_RST;
#if CONFIG_EXAMPLE_LCD_CONTROLLER_SSD1306
  esp_lcd_panel_ssd1306_config_t ssd1306_config = {};
  ssd1306_config.height = EXAMPLE_LCD_V_RES;
  panel_config.vendor_config = &ssd1306_config;
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_ssd1306(io_handle, &panel_config, &panel_handle));
#elif CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
  ESP_ERROR_CHECK(
      esp_lcd_new_panel_sh1107(io_handle, &panel_config, &panel_handle));
#endif

  ESP_ERROR_CHECK(esp_lcd_panel_reset(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_init(panel_handle));
  ESP_ERROR_CHECK(esp_lcd_panel_disp_on_off(panel_handle, true));

#if CONFIG_EXAMPLE_LCD_CONTROLLER_SH1107
  ESP_ERROR_CHECK(esp_lcd_panel_invert_color(panel_handle, true));
#endif

  ESP_LOGI(TAG, "Initialize LVGL");
  lv_init();

  // create a lvgl display
  lv_display_t* raw_display =
      lv_display_create(EXAMPLE_LCD_H_RES, EXAMPLE_LCD_V_RES);

  // Wrap it in C++ class. Note: Using pointer `new` to keep it alive or static.
  // Since app_main task returns, we need it to persist.
  // The 'raw_display' is managed by LVGL, but our wrapper is not.
  // Let's use `new` for the wrapper.
  auto* display = new lvgl::Display(raw_display);

  // associate the i2c panel handle to the display's user data
  // Note: set_user_data on Display wrapper might set it on the raw dict, but we
  // are using it for panel handle. The new set_flush_cb uses user_data to store
  // 'this'. This creates a CONFLICT. We cannot store panel_handle in user_data
  // if Display::set_flush_cb uses it! BUT: The proposed patch in display.cpp
  // showed:
  //   lv_display_set_user_data(disp_, this);
  // So we CANNOT use user_data for panel_handle anymore.
  // We must capture panel_handle in the lambda!

  // create draw buffer
  void* buf = NULL;
  ESP_LOGI(TAG, "Allocate separate LVGL draw buffers");
  // LVGL reserves 2 x 4 bytes in the buffer, as these are assumed to be used as
  // a palette.
  size_t draw_buffer_sz =
      EXAMPLE_LCD_H_RES * EXAMPLE_LCD_V_RES / 8 + EXAMPLE_LVGL_PALETTE_SIZE;
  buf = heap_caps_calloc(1, draw_buffer_sz,
                         MALLOC_CAP_INTERNAL | MALLOC_CAP_8BIT);
  assert(buf);

  // LVGL9 suooprt new monochromatic format.
  display->set_color_format(LV_COLOR_FORMAT_I1);
  // initialize LVGL draw buffers
  display->set_buffers(buf, NULL, draw_buffer_sz, LV_DISPLAY_RENDER_MODE_FULL);
  // set the callback which can copy the rendered image to an area of the
  // display

  // Use lambda to capture panel_handle
  display->set_flush_cb([panel_handle](lvgl::Display* disp,
                                       const lv_area_t* area, uint8_t* px_map) {
    example_lvgl_flush_cb(panel_handle, disp, area, px_map);
  });

  ESP_LOGI(
      TAG,
      "Register io panel event callback for LVGL flush ready notification");
  const esp_lcd_panel_io_callbacks_t cbs = {
      .on_color_trans_done = example_notify_lvgl_flush_ready,
  };
  /* Register done callback */
  // We pass 'display' (the wrapper pointer) as user_ctx so notify_flush_ready
  // can call flush_ready() on it.
  esp_lcd_panel_io_register_event_callbacks(io_handle, &cbs, display);

  ESP_LOGI(TAG, "Use esp_timer as LVGL tick timer");
  const esp_timer_create_args_t lvgl_tick_timer_args = {
      .callback = &example_increase_lvgl_tick,
      .arg = NULL,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "lvgl_tick",
      .skip_unhandled_events = false};
  esp_timer_handle_t lvgl_tick_timer = NULL;
  ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &lvgl_tick_timer));
  ESP_ERROR_CHECK(esp_timer_start_periodic(lvgl_tick_timer,
                                           EXAMPLE_LVGL_TICK_PERIOD_MS * 1000));

  ESP_LOGI(TAG, "Create LVGL task");
  xTaskCreate(example_lvgl_port_task, "LVGL", EXAMPLE_LVGL_TASK_STACK_SIZE,
              NULL, EXAMPLE_LVGL_TASK_PRIORITY, NULL);

  ESP_LOGI(TAG, "Display LVGL Scroll Text");
  // Lock the mutex due to the LVGL APIs are not thread-safe
  _lock_acquire(&lvgl_api_lock);
  example_lvgl_demo_ui(*display);
  _lock_release(&lvgl_api_lock);
}
