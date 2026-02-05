/**
 * @file lv_draw_sw_asm_custom.h
 *
 * This file maps standard LVGL software blending macros to the custom SHIM
 * functions that adapt the data for ESP32 assembly routines.
 */

#ifndef LV_DRAW_SW_ASM_CUSTOM_H
#define LV_DRAW_SW_ASM_CUSTOM_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

// -----------------------------------------------------------------------------
// Shim Function Prototypes
// -----------------------------------------------------------------------------
// We use void* for dsc to avoid dependency on LVGL private headers here.
// The implementation in lv_draw_sw_asm_shim.c handles the casting.

typedef int lv_result_t_esp;  // 1 = OK, 0 = INVALID

lv_result_t_esp lv_color_blend_to_rgb565_shim(const void* dsc);
lv_result_t_esp lv_color_blend_to_rgb888_shim(const void* dsc);
lv_result_t_esp lv_rgb565_blend_normal_to_rgb565_shim(const void* dsc);
lv_result_t_esp lv_rgb888_blend_normal_to_rgb888_shim(const void* dsc);

// -----------------------------------------------------------------------------
// LVGL Hook Macros
// -----------------------------------------------------------------------------

#define LV_DRAW_SW_COLOR_BLEND_TO_RGB565(dsc) lv_color_blend_to_rgb565_shim(dsc)

#define LV_DRAW_SW_COLOR_BLEND_TO_RGB888(dsc, px_size) \
  lv_color_blend_to_rgb888_shim(dsc)

#define LV_DRAW_SW_RGB565_BLEND_NORMAL_TO_RGB565(dsc) \
  lv_rgb565_blend_normal_to_rgb565_shim(dsc)

#define LV_DRAW_SW_RGB888_BLEND_NORMAL_TO_RGB888(dsc, d_size, s_size) \
  lv_rgb888_blend_normal_to_rgb888_shim(dsc)

#ifdef __cplusplus
}
#endif

#endif  // LV_DRAW_SW_ASM_CUSTOM_H
