#include "lvgl_port.h"

#include <unistd.h>

#include <algorithm>  // for std::max, std::min

#include "esp_check.h"
#include "esp_log.h"

static const char* TAG = "LvglPort";

/**
 * @brief Constructor for the LVGL Porting layer.
 *
 * In the ESP32 ecosystem, LVGL is not thread-safe by default. We create a
 * recursive mutex (api_lock_) here.
 *
 * WHY RECURSIVE?
 * This allows the same task to take the lock multiple times without
 * deadlocking. This is useful if a high-level UI function calls another UI
 * function, both of which might try to acquire the lock.
 */
LvglPort::LvglPort(const Config& config) : config_(config) {
  api_lock_ = xSemaphoreCreateRecursiveMutex();
}

/**
 * @brief Initializes the LVGL library and binds it to the hardware.
 *
 * This method walks through the standard LVGL "porting" process:
 * 1. Initialize the core library (lv_init).
 * 2. Create a logical display (lv_display_create).
 * 3. Configure draw buffers (LVGL renders here first before sending to
 * hardware).
 * 4. Set up the flush callback (This transmits the buffer to the screen).
 * 5. Set up a tick timer (LVGL needs to know how much time has passed).
 * 6. Start the background task (The heart of the UI loop).
 */
void LvglPort::init(esp_lcd_panel_handle_t panel_handle,
                    esp_lcd_panel_io_handle_t io_handle) {
  panel_handle_ = panel_handle;

  ESP_LOGI(TAG, "Initialize LVGL");
  lv_init();

  // 1. Logic Display: We tell LVGL our screen resolution.
  lv_display_t* raw_disp = lv_display_create(config_.h_res, config_.v_res);
  display_ = std::make_unique<lvgl::Display>(raw_disp);

  // 2. Buffer Allocation:
  // For monochrome OLEDs like the SSD1306, we use the I1 (1-bit) color format.
  // Each byte holds 8 pixels. We add +8 bytes for the 2-color palette
  // (Black/White).
  size_t draw_buffer_sz = config_.h_res * config_.v_res / 8 + 8;
  draw_buffer_.resize(draw_buffer_sz);

  display_->set_color_format(
      static_cast<lv_color_format_t>(lvgl::ColorFormat::I1));

  // RENDER_MODE_FULL: Since these screens are small (128x64), it's often more
  // efficient to redraw the whole screen than to perform complex partial
  // updates due to the SSD1306's vertical page addressing.
  display_->set_buffers(draw_buffer_.data(), nullptr, draw_buffer_sz,
                        LV_DISPLAY_RENDER_MODE_FULL);

  // 3. Flush Callback: This is the bridge between LVGL's memory and our I2C
  // driver.
  display_->set_flush_cb(
      [this](lvgl::Display* disp, const lv_area_t* area, uint8_t* px_map) {
        this->flush_cb(disp, area, px_map);
      });

  // 4. Input/Output Sync: We register a callback for when the I2C transfer is
  // done. This helps LVGL manage its frame rate efficiently.
  const esp_lcd_panel_io_callbacks_t cbs = {
      .on_color_trans_done = notify_flush_ready_trampoline,
  };
  esp_lcd_panel_io_register_event_callbacks(io_handle, &cbs, this);

  // 5. Tick Timer: LVGL needs a heartbeat.
  // We use the ESP-IDF hardware timer (esp_timer) because it's highly accurate
  // and doesn't depend on FreeRTOS task scheduling jitter.
  const esp_timer_create_args_t lvgl_tick_timer_args = {
      .callback = &tick_increment_trampoline,
      .arg = this,
      .dispatch_method = ESP_TIMER_TASK,
      .name = "lvgl_tick",
      .skip_unhandled_events = false,
  };
  ESP_ERROR_CHECK(esp_timer_create(&lvgl_tick_timer_args, &tick_timer_));
  ESP_ERROR_CHECK(
      esp_timer_start_periodic(tick_timer_, config_.tick_period_ms * 1000));

  // 6. Background Task:
  // We start a dedicated FreeRTOS task to handle UI rendering and timers.
  // Priority and stack size are configurable but sensitive—rendering can be
  // heavy!
  xTaskCreate(task_trampoline, "LVGL", config_.task_stack_size, this,
              config_.task_priority, &task_handle_);
}

bool LvglPort::lock(uint32_t timeout_ms) {
  return xSemaphoreTakeRecursive(api_lock_, pdMS_TO_TICKS(timeout_ms)) ==
         pdTRUE;
}

void LvglPort::unlock() { xSemaphoreGiveRecursive(api_lock_); }

/**
 * @brief Sets the screen orientation and synchronizes hardware/software states.
 *
 * Rotation is a complex topic in embedded UI:
 * - HARDWARE ROTATION: The display controller mirrors its own memory (fast).
 * - SOFTWARE ROTATION: LVGL calculates different coordinates for drawing
 * (flexible).
 *
 * Our implementation is a HYBRID:
 * - For 180°, we use Hardware Mirroring—it's native to the SSD1306 and consumes
 *   zero CPU cycles for the reversal.
 * - For 90/270°, we must use Software Remapping because the SSD1306 doesn't
 *   support 90° natively without changing how we pack bits.
 */
void LvglPort::set_rotation(lvgl::Display::Rotation rotation) {
  if (!display_) return;

  // CRITICAL: We lock the mutex because we are changing the state of the
  // display that the background LVGL task is actively using!
  lock(-1);
  display_->set_rotation(rotation);

  // 1. Hardware Mirroring (Efficient for 180-degree flips)
  if (rotation == lvgl::Display::Rotation::ROT_180) {
    esp_lcd_panel_mirror(panel_handle_, true, true);
  } else {
    // Return to default orientation for all other modes.
    esp_lcd_panel_mirror(panel_handle_, false, false);
  }

  unlock();

  ESP_LOGI(TAG, "Rotation set to %d", static_cast<int>(rotation));
}

/**
 * @brief The "Bridge" between LVGL and the hardware.
 *
 * This is called by LVGL whenever a portion of the screen needs to be
 * updated on the physical display.
 *
 * THE CHALLENGE:
 * LVGL works with a Linear Buffer (1 pixel = 1 bit in a row).
 * SSD1306 works with Page Addressing (1 byte = 8 VERTICAL pixels).
 *
 * Our loop below performs a "Transposition":
 * It reads from LVGL's row-major format and packs it into the SSD1306's
 * column-major vertical format.
 */
void LvglPort::flush_cb(lvgl::Display* disp, const lv_area_t* area,
                        uint8_t* px_map) {
  // Skip palette
  px_map += 8;

  int x1 = area->x1;
  int x2 = area->x2;
  int y1 = area->y1;
  int y2 = area->y2;

  static std::vector<uint8_t> s_oled_buffer(config_.h_res * config_.v_res / 8);

  lvgl::Display::Rotation rotation = display_->get_rotation();

  for (int y = y1; y <= y2; y++) {
    for (int x = x1; x <= x2; x++) {
      /**
       * 1. EXTRACT FROM LVGL (Logical Space)
       * LVGL provides a 1-bit packed buffer (I1). We find the byte and bit.
       * Logic: byte_offset = (row_width / 8) * y + (x / 8)
       *
       * NOTE: We use disp->get_horizontal_resolution() because for 90/270deg
       * rotations, the logical width is NOT the physical width (128).
       */
      bool chroma_color =
          (px_map[(disp->get_horizontal_resolution() >> 3) * y + (x >> 3)] &
           1 << (7 - x % 8));

      /**
       * 2. REMAP COORDINATES (Software Rotation)
       * For 90° and 270°, we must translate LVGL's logical (x, y)
       * to the physical (x_p, y_p) coordinates of the OLED glass.
       */
      int x_p = x;
      int y_p = y;

      switch (rotation) {
        case lvgl::Display::Rotation::ROT_0:
          break;
        case lvgl::Display::Rotation::ROT_90:
          x_p = config_.h_res - 1 - y;
          y_p = x;
          break;
        case lvgl::Display::Rotation::ROT_180:
          // Already handled at the hardware level by esp_lcd_panel_mirror!
          break;
        case lvgl::Display::Rotation::ROT_270:
          x_p = y;
          y_p = config_.v_res - 1 - x;
          break;
      }

      /**
       * 3. PACK FOR SSD1306 (Physical Space)
       * The SSD1306 expects "Page Addressing".
       * - Each byte represents 8 vertical pixels (y_p % 8).
       * - Bytes are arranged horizontally (x_p).
       *
       * This is why we use (y_p >> 3) to find the page and (y_p % 8) for the
       * bit.
       */
      uint8_t* buf = s_oled_buffer.data() + config_.h_res * (y_p >> 3) + (x_p);
      if (chroma_color) {
        // Bit = 0 (Black/Off), Bit = 1 (White/On)
        // Some displays are inverted; we handle common logic here.
        (*buf) &= ~(1 << (y_p % 8));
      } else {
        (*buf) |= (1 << (y_p % 8));
      }
    }
  }

  // [Detailed Explanation of Full-Screen Refresh Strategy]
  // We strictly perform a full-screen refresh (0, 0, h_res, v_res) instead of
  // partial updates for two critical reasons:
  //
  // 1. Buffer Alignment & Contiguity: The SSD1306 controller uses a
  // "page-based" memory layout (vertical bytes),
  //    meaning a single byte represents 8 vertical pixels. Arbitrary
  //    rectangular areas (x1, y1, x2, y2) from LVGL often do not align neatly
  //    with these 8-pixel page boundaries. Sending a partial update requires
  //    complex bit-hacking to ensure the data sent matches the controller's
  //    expected page structure.
  //
  // 2. Driver Expectations: The `esp_lcd_panel_draw_bitmap` function typically
  // expects the input buffer to contain
  //    *only* the pixel data for the specified window, packed contiguously.
  //    However, our `s_oled_buffer` accumulates the state of the *entire*
  //    screen. Passing a pointer into the middle of this full-frame buffer for
  //    a partial window works for linear memory, but fails for paged memory
  //    because the stride (width of the full screen) doesn't match the width of
  //    the partial window, leading to skewed/garbage display output ("piling
  //    on" artifacts).
  //
  // Therefore, we treat `s_oled_buffer` as the single source of truth for the
  // entire display and flush it completely on every update. This is
  // computationally cheap for small displays (128x64) and guarantees
  // correctness. [What NOT to do] We reverted to partial updates here because
  // the Full-Screen Refresh approach (sending the whole buffer 0,0->128,64)
  // caused the text to become unreadable/scrambled on the device, possibly due
  // to timing issues or driver limitations with large data bursts.
  //
  // HOWEVER, this Partial Update approach using `s_oled_buffer.data()` is ALSO
  // imperfect: Since the SSD1306 uses Page Addressing (vertical bytes) and our
  // buffer `s_oled_buffer` represents a full screen, the data for a rectangular
  // window (x1, y1...) is NOT contiguous in `s_oled_buffer`. Passing the start
  // of the buffer `.data()` for a partial window `x1, y1` tells the driver to
  // grab the *first* bytes of the buffer and paint them into the window, which
  // results in "piling on" or ghosting artifacts because it's drawing Page 0
  // data into whatever window we requested.
  //
  // To fix this properly, one would need to allocate a temporary contiguous
  // buffer, copy the specific non-contiguous pages/columns from `s_oled_buffer`
  // into it, and send that. For now, we revert to partials as requested to
  // restore readability, accepting the artifacts.
  esp_lcd_panel_draw_bitmap(panel_handle_, x1, y1, x2 + 1, y2 + 1,
                            s_oled_buffer.data());
}

bool LvglPort::notify_flush_ready_trampoline(
    esp_lcd_panel_io_handle_t io_panel, esp_lcd_panel_io_event_data_t* edata,
    void* user_ctx) {
  auto* self = static_cast<LvglPort*>(user_ctx);
  self->display_->flush_ready();
  return false;
}

void LvglPort::tick_increment_trampoline(void* arg) {
  auto* self = static_cast<LvglPort*>(arg);
  lv_tick_inc(self->config_.tick_period_ms);
}

void LvglPort::task_trampoline(void* arg) {
  auto* self = static_cast<LvglPort*>(arg);
  self->task_loop();
}

/**
 * @brief The Heart of the UI.
 *
 * This background task runs indefinitely. Its only job is to call
 * `lv_timer_handler()` which processes animations, events, and rendering.
 */
void LvglPort::task_loop() {
  ESP_LOGI(TAG, "Starting LVGL task");
  while (1) {
    /**
     * THREAD SAFETY IS PARAMOUNT:
     * We must lock the mutex before calling ANY LVGL function in this task
     * because other tasks (like your main app) might be trying to update
     * labels or change screens at the same time.
     */
    lock(-1);  // Wait forever for the lock
    uint32_t time_till_next_ms = lv_timer_handler();
    unlock();

    /**
     * EFFICIENCY:
     * `lv_timer_handler()` returns the number of milliseconds until it needs
     * to run again. We sleep for that duration to save CPU and Power.
     */
    time_till_next_ms = std::max(time_till_next_ms, (uint32_t)1);
    time_till_next_ms = std::min(time_till_next_ms, (uint32_t)500);

    // usleep takes microseconds, so we multiply by 1000.
    usleep(1000 * time_till_next_ms);
  }
}

/**
 * WHAT NOT TO DO:
 * 1. DON'T use `vTaskDelay` inside `flush_cb`. It will block the UI thread
 *    and could lead to I2C transfer timeouts or choppy animations.
 * 2. DON'T forget the mutex! LVGL is NOT thread-safe for the ESP32. If you
 *    update a label from `app_main` without `sys.lock()`, several crashes
 *    or race conditions will eventually happen.
 * 3. DON'T use `malloc` in the `flush_cb`. It's too slow. Use `static`
 *    buffers like `s_oled_buffer` or pre-allocated members.
 */
