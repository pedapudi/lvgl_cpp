#include "simd_plugin.h"

#if defined(CONFIG_IDF_TARGET_ESP32S3)

#include "lv_draw_private.h"
#include "lv_draw_sw.h"
#include "lv_area_private.h"
#include "lv_draw_sw_blend_private.h"
#include "lv_draw_sw_blend_to_rgb565.h"
#include "lv_draw_sw_blend_to_rgb888.h"

namespace lvgl {
namespace utility {

// -----------------------------------------------------------------------------
// 1. ESP Assembly Struct Definition (Mirror for Assembly Safety)
// -----------------------------------------------------------------------------
typedef struct {
  uint32_t opa;              // 0
  void* dst_buf;             // 4
  uint32_t dst_w;            // 8
  uint32_t dst_h;            // 12
  uint32_t dst_stride;       // 16
  const void* src_buf;       // 20
  uint32_t src_stride;       // 24
  const lv_opa_t* mask_buf;  // 28
  uint32_t mask_stride;      // 32
} esp_asm_dsc_t;

// Extern Assembly Functions
extern "C" {
int lv_color_blend_to_rgb565_esp(const esp_asm_dsc_t* dsc);
int lv_color_blend_to_rgb888_esp(const esp_asm_dsc_t* dsc);
int lv_rgb565_blend_normal_to_rgb565_esp(const esp_asm_dsc_t* dsc);
int lv_rgb888_blend_normal_to_rgb888_esp(const esp_asm_dsc_t* dsc);
}

void Esp32S3SimdPlugin::apply() {
  static lv_draw_sw_custom_blend_handler_t handler_rgb565;
  handler_rgb565.dest_cf = LV_COLOR_FORMAT_RGB565;
  handler_rgb565.handler = blend_handler_rgb565;
  lv_draw_sw_register_blend_handler(&handler_rgb565);

  static lv_draw_sw_custom_blend_handler_t handler_rgb888;
  handler_rgb888.dest_cf = LV_COLOR_FORMAT_RGB888;
  handler_rgb888.handler = blend_handler_rgb888;
  lv_draw_sw_register_blend_handler(&handler_rgb888);

  static lv_draw_sw_custom_blend_handler_t handler_xrgb8888;
  handler_xrgb8888.dest_cf = LV_COLOR_FORMAT_XRGB8888;
  handler_xrgb8888.handler = blend_handler_rgb888;
  lv_draw_sw_register_blend_handler(&handler_xrgb8888);
}

void Esp32S3SimdPlugin::blend_handler_rgb565(
    lv_draw_task_t* t, const lv_draw_sw_blend_dsc_t* blend_dsc) {
  // Setup logic mirrored from lv_draw_sw_blend.c
  lv_area_t blend_area;
  if (!lv_area_intersect(&blend_area, blend_dsc->blend_area, &t->clip_area))
    return;

  lv_layer_t* layer = t->target_layer;
  uint32_t layer_stride_byte = layer->draw_buf->header.stride;

  if (blend_dsc->src_buf == nullptr) {
    // 1. Color Fill CASE
    lv_draw_sw_blend_fill_dsc_t fill_dsc;
    fill_dsc.dest_w = lv_area_get_width(&blend_area);
    fill_dsc.dest_h = lv_area_get_height(&blend_area);
    fill_dsc.dest_stride = layer_stride_byte;
    fill_dsc.opa = blend_dsc->opa;
    fill_dsc.color = blend_dsc->color;
    fill_dsc.mask_stride = 0;

    if (blend_dsc->mask_buf == nullptr ||
        blend_dsc->mask_res == LV_DRAW_SW_MASK_RES_FULL_COVER) {
      fill_dsc.mask_buf = nullptr;
    } else {
      fill_dsc.mask_buf = blend_dsc->mask_buf;
      fill_dsc.mask_stride = blend_dsc->mask_stride == 0
                                 ? lv_area_get_width(blend_dsc->mask_area)
                                 : blend_dsc->mask_stride;
      fill_dsc.mask_buf +=
          fill_dsc.mask_stride * (blend_area.y1 - blend_dsc->mask_area->y1) +
          (blend_area.x1 - blend_dsc->mask_area->x1);
    }

    fill_dsc.relative_area = blend_area;
    lv_area_move(&fill_dsc.relative_area, -layer->buf_area.x1,
                 -layer->buf_area.y1);
    fill_dsc.dest_buf =
        lv_draw_layer_go_to_xy(layer, blend_area.x1 - layer->buf_area.x1,
                               blend_area.y1 - layer->buf_area.y1);

    // Filter for SIMD case (Normal blend mode, No complex mask requirements)
    if (blend_dsc->blend_mode == LV_BLEND_MODE_NORMAL) {
      esp_asm_dsc_t asm_dsc;
      asm_dsc.opa = fill_dsc.opa;
      asm_dsc.dst_buf = fill_dsc.dest_buf;
      asm_dsc.dst_w = fill_dsc.dest_w;
      asm_dsc.dst_h = fill_dsc.dest_h;
      asm_dsc.dst_stride = fill_dsc.dest_stride;

      lv_color32_t c32 = lv_color_to_32(fill_dsc.color, 0xFF);
      asm_dsc.src_buf =
          (void*)&c32;  // Color fill passes a pointer to a color32
      asm_dsc.src_stride = 0;
      asm_dsc.mask_buf = fill_dsc.mask_buf;
      asm_dsc.mask_stride = fill_dsc.mask_stride;

      if (asm_dsc.mask_buf == nullptr && lv_color_blend_to_rgb565_esp(&asm_dsc)) return;  // Handled
    }

    // Fallback
    lv_draw_sw_blend_color_to_rgb565(&fill_dsc);
  } else {
    // 2. Image Blend CASE
    if (!lv_area_intersect(&blend_area, &blend_area, blend_dsc->src_area))
      return;
    if (blend_dsc->mask_area &&
        !lv_area_intersect(&blend_area, &blend_area, blend_dsc->mask_area))
      return;

    lv_draw_sw_blend_image_dsc_t image_dsc;
    image_dsc.dest_w = lv_area_get_width(&blend_area);
    image_dsc.dest_h = lv_area_get_height(&blend_area);
    image_dsc.dest_stride = layer_stride_byte;

    image_dsc.opa = blend_dsc->opa;
    image_dsc.blend_mode = blend_dsc->blend_mode;
    image_dsc.src_stride = blend_dsc->src_stride;
    image_dsc.src_color_format = blend_dsc->src_color_format;

    const uint8_t* src_buf = (const uint8_t*)blend_dsc->src_buf;
    uint32_t src_px_size = lv_color_format_get_bpp(blend_dsc->src_color_format);
    src_buf += image_dsc.src_stride * (blend_area.y1 - blend_dsc->src_area->y1);
    src_buf += ((blend_area.x1 - blend_dsc->src_area->x1) * src_px_size) >> 3;
    image_dsc.src_buf = src_buf;
    image_dsc.mask_stride = 0;

    if (blend_dsc->mask_buf == nullptr ||
        blend_dsc->mask_res == LV_DRAW_SW_MASK_RES_FULL_COVER) {
      image_dsc.mask_buf = nullptr;
    } else {
      image_dsc.mask_buf = blend_dsc->mask_buf;
      image_dsc.mask_stride = blend_dsc->mask_stride
                                  ? blend_dsc->mask_stride
                                  : lv_area_get_width(blend_dsc->mask_area);
      image_dsc.mask_buf +=
          image_dsc.mask_stride * (blend_area.y1 - blend_dsc->mask_area->y1) +
          (blend_area.x1 - blend_dsc->mask_area->x1);
    }

    image_dsc.relative_area = blend_area;
    lv_area_move(&image_dsc.relative_area, -layer->buf_area.x1,
                 -layer->buf_area.y1);
    image_dsc.src_area = *blend_dsc->src_area;
    lv_area_move(&image_dsc.src_area, -layer->buf_area.x1, -layer->buf_area.y1);
    image_dsc.dest_buf =
        lv_draw_layer_go_to_xy(layer, blend_area.x1 - layer->buf_area.x1,
                               blend_area.y1 - layer->buf_area.y1);

    // Filter for SIMD case (Normal blend mode, Specific source formats)
    if (image_dsc.blend_mode == LV_BLEND_MODE_NORMAL) {
      if (image_dsc.src_color_format == LV_COLOR_FORMAT_RGB565) {
        esp_asm_dsc_t asm_dsc;
        asm_dsc.opa = image_dsc.opa;
        asm_dsc.dst_buf = image_dsc.dest_buf;
        asm_dsc.dst_w = image_dsc.dest_w;
        asm_dsc.dst_h = image_dsc.dest_h;
        asm_dsc.dst_stride = image_dsc.dest_stride;
        asm_dsc.src_buf = image_dsc.src_buf;
        asm_dsc.src_stride = image_dsc.src_stride;
        asm_dsc.mask_buf = image_dsc.mask_buf;
        asm_dsc.mask_stride = image_dsc.mask_stride;

        if (asm_dsc.mask_buf == nullptr && lv_rgb565_blend_normal_to_rgb565_esp(&asm_dsc)) return;  // Handled
      }
    }

    // Fallback
    lv_draw_sw_blend_image_to_rgb565(&image_dsc);
  }
}

void Esp32S3SimdPlugin::blend_handler_rgb888(
    lv_draw_task_t* t, const lv_draw_sw_blend_dsc_t* blend_dsc) {
  uint32_t dest_px_size =
      lv_color_format_get_size(t->target_layer->color_format);

  lv_area_t blend_area;
  if (!lv_area_intersect(&blend_area, blend_dsc->blend_area, &t->clip_area))
    return;

  lv_layer_t* layer = t->target_layer;
  uint32_t layer_stride_byte = layer->draw_buf->header.stride;

  if (blend_dsc->src_buf == nullptr) {
    // 1. Color Fill CASE
    lv_draw_sw_blend_fill_dsc_t fill_dsc;
    fill_dsc.dest_w = lv_area_get_width(&blend_area);
    fill_dsc.dest_h = lv_area_get_height(&blend_area);
    fill_dsc.dest_stride = layer_stride_byte;
    fill_dsc.opa = blend_dsc->opa;
    fill_dsc.color = blend_dsc->color;
    fill_dsc.mask_stride = 0;

    if (blend_dsc->mask_buf == nullptr ||
        blend_dsc->mask_res == LV_DRAW_SW_MASK_RES_FULL_COVER) {
      fill_dsc.mask_buf = nullptr;
    } else {
      fill_dsc.mask_buf = blend_dsc->mask_buf;
      fill_dsc.mask_stride = blend_dsc->mask_stride == 0
                                 ? lv_area_get_width(blend_dsc->mask_area)
                                 : blend_dsc->mask_stride;
      fill_dsc.mask_buf +=
          fill_dsc.mask_stride * (blend_area.y1 - blend_dsc->mask_area->y1) +
          (blend_area.x1 - blend_dsc->mask_area->x1);
    }

    fill_dsc.relative_area = blend_area;
    lv_area_move(&fill_dsc.relative_area, -layer->buf_area.x1,
                 -layer->buf_area.y1);
    fill_dsc.dest_buf =
        lv_draw_layer_go_to_xy(layer, blend_area.x1 - layer->buf_area.x1,
                               blend_area.y1 - layer->buf_area.y1);

    // Filter for SIMD case
    if (blend_dsc->blend_mode == LV_BLEND_MODE_NORMAL && dest_px_size == 3) {
      esp_asm_dsc_t asm_dsc;
      asm_dsc.opa = fill_dsc.opa;
      asm_dsc.dst_buf = fill_dsc.dest_buf;
      asm_dsc.dst_w = fill_dsc.dest_w;
      asm_dsc.dst_h = fill_dsc.dest_h;
      asm_dsc.dst_stride = fill_dsc.dest_stride;

      lv_color32_t c32 = lv_color_to_32(fill_dsc.color, 0xFF);
      asm_dsc.src_buf = (void*)&c32;
      asm_dsc.src_stride = 0;
      asm_dsc.mask_buf = fill_dsc.mask_buf;
      asm_dsc.mask_stride = fill_dsc.mask_stride;

      if (asm_dsc.mask_buf == nullptr && lv_color_blend_to_rgb888_esp(&asm_dsc)) return;
    }

    // Fallback
    lv_draw_sw_blend_color_to_rgb888(&fill_dsc, dest_px_size);
  } else {
    // 2. Image Blend CASE
    if (!lv_area_intersect(&blend_area, &blend_area, blend_dsc->src_area))
      return;
    if (blend_dsc->mask_area &&
        !lv_area_intersect(&blend_area, &blend_area, blend_dsc->mask_area))
      return;

    lv_draw_sw_blend_image_dsc_t image_dsc;
    image_dsc.dest_w = lv_area_get_width(&blend_area);
    image_dsc.dest_h = lv_area_get_height(&blend_area);
    image_dsc.dest_stride = layer_stride_byte;

    image_dsc.opa = blend_dsc->opa;
    image_dsc.blend_mode = blend_dsc->blend_mode;
    image_dsc.src_stride = blend_dsc->src_stride;
    image_dsc.src_color_format = blend_dsc->src_color_format;

    const uint8_t* src_buf = (const uint8_t*)blend_dsc->src_buf;
    uint32_t src_px_size = lv_color_format_get_bpp(blend_dsc->src_color_format);
    src_buf += image_dsc.src_stride * (blend_area.y1 - blend_dsc->src_area->y1);
    src_buf += ((blend_area.x1 - blend_dsc->src_area->x1) * src_px_size) >> 3;
    image_dsc.src_buf = src_buf;
    image_dsc.mask_stride = 0;

    if (blend_dsc->mask_buf == nullptr ||
        blend_dsc->mask_res == LV_DRAW_SW_MASK_RES_FULL_COVER) {
      image_dsc.mask_buf = nullptr;
    } else {
      image_dsc.mask_buf = blend_dsc->mask_buf;
      image_dsc.mask_stride = blend_dsc->mask_stride
                                  ? blend_dsc->mask_stride
                                  : lv_area_get_width(blend_dsc->mask_area);
      image_dsc.mask_buf +=
          image_dsc.mask_stride * (blend_area.y1 - blend_dsc->mask_area->y1) +
          (blend_area.x1 - blend_dsc->mask_area->x1);
    }

    image_dsc.relative_area = blend_area;
    lv_area_move(&image_dsc.relative_area, -layer->buf_area.x1,
                 -layer->buf_area.y1);
    image_dsc.src_area = *blend_dsc->src_area;
    lv_area_move(&image_dsc.src_area, -layer->buf_area.x1, -layer->buf_area.y1);
    image_dsc.dest_buf =
        lv_draw_layer_go_to_xy(layer, blend_area.x1 - layer->buf_area.x1,
                               blend_area.y1 - layer->buf_area.y1);

    // Filter for SIMD case
    if (image_dsc.blend_mode == LV_BLEND_MODE_NORMAL && dest_px_size == 3) {
      if (image_dsc.src_color_format == LV_COLOR_FORMAT_RGB888) {
        esp_asm_dsc_t asm_dsc;
        asm_dsc.opa = image_dsc.opa;
        asm_dsc.dst_buf = image_dsc.dest_buf;
        asm_dsc.dst_w = image_dsc.dest_w;
        asm_dsc.dst_h = image_dsc.dest_h;
        asm_dsc.dst_stride = image_dsc.dest_stride;
        asm_dsc.src_buf = image_dsc.src_buf;
        asm_dsc.src_stride = image_dsc.src_stride;
        asm_dsc.mask_buf = image_dsc.mask_buf;
        asm_dsc.mask_stride = image_dsc.mask_stride;

        if (asm_dsc.mask_buf == nullptr && lv_rgb888_blend_normal_to_rgb888_esp(&asm_dsc)) return;
      }
    }

    // Fallback
    lv_draw_sw_blend_image_to_rgb888(&image_dsc, dest_px_size);
  }
}

}  // namespace utility
}  // namespace lvgl

#endif  // CONFIG_IDF_TARGET_ESP32S3
