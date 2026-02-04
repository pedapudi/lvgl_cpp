/**
 * @file lv_draw_sw_asm_shim.c
 *
 * Implements the shim functions to bridge LVGL 9.4 C structs to ESP32 Assembly
 * structs.
 */

#include "lvgl.h" // Pull in public types (lv_color_t, lv_area_t, etc.)

// -----------------------------------------------------------------------------
// 1. ESP Assembly Struct Definition
// -----------------------------------------------------------------------------
typedef struct {
  uint32_t opa;             // 0
  void *dst_buf;            // 4
  uint32_t dst_w;           // 8
  uint32_t dst_h;           // 12
  uint32_t dst_stride;      // 16
  const void *src_buf;      // 20
  uint32_t src_stride;      // 24
  const lv_opa_t *mask_buf; // 28
  uint32_t mask_stride;     // 32
} esp_asm_dsc_t;

typedef int lv_result_t_esp;

// Extern Assembly Functions
extern lv_result_t_esp lv_color_blend_to_rgb565_esp(const esp_asm_dsc_t *dsc);
extern lv_result_t_esp lv_color_blend_to_rgb888_esp(const esp_asm_dsc_t *dsc);
extern lv_result_t_esp
lv_rgb565_blend_normal_to_rgb565_esp(const esp_asm_dsc_t *dsc);
extern lv_result_t_esp
lv_rgb888_blend_normal_to_rgb888_esp(const esp_asm_dsc_t *dsc);

// -----------------------------------------------------------------------------
// 2. Local Definition of LVGL Private Structs (Mirrored from LVGL 9.4 source)
// -----------------------------------------------------------------------------
// We define them here to avoid "incomplete type" errors from missing private
// headers.

typedef struct {
  void *dest_buf;
  int32_t dest_w;
  int32_t dest_h;
  int32_t dest_stride;
  const lv_opa_t *mask_buf;
  int32_t mask_stride;
  lv_color_t color;
  lv_opa_t opa;
  lv_area_t relative_area;
} shim_lv_draw_sw_blend_fill_dsc_t;

typedef struct {
  void *dest_buf;
  int32_t dest_w;
  int32_t dest_h;
  int32_t dest_stride;
  const lv_opa_t *mask_buf;
  int32_t mask_stride;
  const void *src_buf;
  int32_t src_stride;
  lv_color_format_t src_color_format;
  lv_opa_t opa;
  lv_blend_mode_t blend_mode;
  lv_area_t relative_area;
  lv_area_t src_area;
} shim_lv_draw_sw_blend_image_dsc_t;

// -----------------------------------------------------------------------------
// 3. Shim Implementations
// -----------------------------------------------------------------------------

lv_result_t_esp lv_color_blend_to_rgb565_shim(const void *dsc_void) {
  const shim_lv_draw_sw_blend_fill_dsc_t *dsc =
      (const shim_lv_draw_sw_blend_fill_dsc_t *)dsc_void;
  esp_asm_dsc_t asm_dsc;

  asm_dsc.opa = dsc->opa;
  asm_dsc.dst_buf = dsc->dest_buf;
  asm_dsc.dst_w = dsc->dest_w;
  asm_dsc.dst_h = dsc->dest_h;
  asm_dsc.dst_stride = dsc->dest_stride;

  // Convert 16-bit color to 32-bit for assembly safety
  lv_color32_t c32 = lv_color_to_32(dsc->color, 0xFF);
  asm_dsc.src_buf = &c32;

  asm_dsc.src_stride = 0;
  asm_dsc.mask_buf = dsc->mask_buf;
  asm_dsc.mask_stride = dsc->mask_stride;

  return lv_color_blend_to_rgb565_esp(&asm_dsc);
}

lv_result_t_esp lv_color_blend_to_rgb888_shim(const void *dsc_void) {
  const shim_lv_draw_sw_blend_fill_dsc_t *dsc =
      (const shim_lv_draw_sw_blend_fill_dsc_t *)dsc_void;
  esp_asm_dsc_t asm_dsc;

  asm_dsc.opa = dsc->opa;
  asm_dsc.dst_buf = dsc->dest_buf;
  asm_dsc.dst_w = dsc->dest_w;
  asm_dsc.dst_h = dsc->dest_h;
  asm_dsc.dst_stride = dsc->dest_stride;

  // Convert to 32-bit color
  lv_color32_t c32 = lv_color_to_32(dsc->color, 0xFF);
  asm_dsc.src_buf = &c32;

  asm_dsc.src_stride = 0;
  asm_dsc.mask_buf = dsc->mask_buf;
  asm_dsc.mask_stride = dsc->mask_stride;

  return lv_color_blend_to_rgb888_esp(&asm_dsc);
}

lv_result_t_esp lv_rgb565_blend_normal_to_rgb565_shim(const void *dsc_void) {
  const shim_lv_draw_sw_blend_image_dsc_t *dsc =
      (const shim_lv_draw_sw_blend_image_dsc_t *)dsc_void;
  esp_asm_dsc_t asm_dsc;

  asm_dsc.opa = dsc->opa;
  asm_dsc.dst_buf = dsc->dest_buf;
  asm_dsc.dst_w = dsc->dest_w;
  asm_dsc.dst_h = dsc->dest_h;
  asm_dsc.dst_stride = dsc->dest_stride;
  asm_dsc.src_buf = dsc->src_buf;
  asm_dsc.src_stride = dsc->src_stride;
  asm_dsc.mask_buf = dsc->mask_buf;
  asm_dsc.mask_stride = dsc->mask_stride;

  return lv_rgb565_blend_normal_to_rgb565_esp(&asm_dsc);
}

lv_result_t_esp lv_rgb888_blend_normal_to_rgb888_shim(const void *dsc_void) {
  const shim_lv_draw_sw_blend_image_dsc_t *dsc =
      (const shim_lv_draw_sw_blend_image_dsc_t *)dsc_void;
  esp_asm_dsc_t asm_dsc;

  asm_dsc.opa = dsc->opa;
  asm_dsc.dst_buf = dsc->dest_buf;
  asm_dsc.dst_w = dsc->dest_w;
  asm_dsc.dst_h = dsc->dest_h;
  asm_dsc.dst_stride = dsc->dest_stride;
  asm_dsc.src_buf = dsc->src_buf;
  asm_dsc.src_stride = dsc->src_stride;
  asm_dsc.mask_buf = dsc->mask_buf;
  asm_dsc.mask_stride = dsc->mask_stride;

  return lv_rgb888_blend_normal_to_rgb888_esp(&asm_dsc);
}
