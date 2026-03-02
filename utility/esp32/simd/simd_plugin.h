#ifndef LVGL_CPP_UTILITY_ESP32_SIMD_PLUGIN_H_
#define LVGL_CPP_UTILITY_ESP32_SIMD_PLUGIN_H_

#include "lvgl.h"

namespace lvgl {
namespace utility {

/**
 * @brief Manages registration of custom ESP32-S3 SIMD blend handlers.
 *
 * In LVGL 9.5+, custom optimizations are registered at runtime via the
 * handler API rather than library-wide macro hijacking.
 */
class Esp32S3SimdPlugin {
 public:
  /**
   * @brief Register the SIMD handlers for supported color formats.
   */
  static void apply();

 private:
  /** @brief Blend handler for RGB565 destination */
  static void blend_handler_rgb565(lv_draw_task_t* t,
                                   const lv_draw_sw_blend_dsc_t* dsc);

  /** @brief Blend handler for RGB888 / XRGB8888 destination */
  static void blend_handler_rgb888(lv_draw_task_t* t,
                                   const lv_draw_sw_blend_dsc_t* dsc);
};

}  // namespace utility
}  // namespace lvgl

#endif  // LVGL_CPP_UTILITY_ESP32_SIMD_PLUGIN_H_
