#include "draw_buf.h"

#include <utility>
#if defined(ESP_PLATFORM)
#include "esp_heap_caps.h"
#endif

namespace lvgl {
namespace draw {

DrawBuf::DrawBuf(uint32_t w, uint32_t h, ColorFormat cf, uint32_t stride)
    : buf_(
          lv_draw_buf_create(w, h, static_cast<lv_color_format_t>(cf), stride)),
      owns_(true),
      deallocator_(nullptr) {}

#if defined(ESP_PLATFORM)
static void esp32_dma_deallocator(lv_draw_buf_t* buf) {
  if (buf) {
    if (buf->data) {
      heap_caps_free(buf->data);
    }
    lv_free(buf);
  }
}
#endif

DrawBuf DrawBuf::allocate_dma(uint32_t w, uint32_t h, ColorFormat cf,
                              uint32_t caps) {
#if defined(ESP_PLATFORM)
  lv_color_format_t lv_cf = static_cast<lv_color_format_t>(cf);
  uint32_t stride = lv_draw_buf_width_to_stride(w, lv_cf);
  size_t size = stride * h;

  // Default to internal DMA if nothing specified
  if (caps == 0) caps = MALLOC_CAP_DMA | MALLOC_CAP_INTERNAL;

  // Use 64-byte alignment for S3 cache safety
  void* data = heap_caps_aligned_alloc(64, size, caps);
  if (!data) return DrawBuf(nullptr);

  // Create the draw buffer structure
  lv_draw_buf_t* buf =
      static_cast<lv_draw_buf_t*>(lv_malloc(sizeof(lv_draw_buf_t)));
  if (!buf) {
    heap_caps_free(data);
    return DrawBuf(nullptr);
  }

  lv_draw_buf_init(buf, w, h, lv_cf, stride, data, size);
  DrawBuf db(buf, true);
  db.set_deallocator(esp32_dma_deallocator);
  return db;
#else
  return DrawBuf(w, h, cf);
#endif
}

DrawBuf::DrawBuf(lv_draw_buf_t* buf, bool take_ownership)
    : buf_(buf), owns_(take_ownership), deallocator_(nullptr) {}

DrawBuf::~DrawBuf() {
  if (buf_ && owns_) {
    if (deallocator_) {
      deallocator_(buf_);
    } else {
      lv_draw_buf_destroy(buf_);
    }
  }
}

void DrawBuf::set_deallocator(void (*deallocator)(lv_draw_buf_t*)) {
  deallocator_ = deallocator;
}

DrawBuf::DrawBuf(DrawBuf&& other) noexcept
    : buf_(other.buf_), owns_(other.owns_), deallocator_(other.deallocator_) {
  other.buf_ = nullptr;
  other.owns_ = false;
  other.deallocator_ = nullptr;
}

DrawBuf& DrawBuf::operator=(DrawBuf&& other) noexcept {
  if (this != &other) {
    if (buf_ && owns_) {
      if (deallocator_) {
        deallocator_(buf_);
      } else {
        lv_draw_buf_destroy(buf_);
      }
    }
    buf_ = other.buf_;
    owns_ = other.owns_;
    deallocator_ = other.deallocator_;
    other.buf_ = nullptr;
    other.owns_ = false;
    other.deallocator_ = nullptr;
  }
  return *this;
}

uint32_t DrawBuf::width() const { return buf_ ? buf_->header.w : 0; }

uint32_t DrawBuf::height() const { return buf_ ? buf_->header.h : 0; }

ColorFormat DrawBuf::format() const {
  return buf_ ? static_cast<ColorFormat>(buf_->header.cf)
              : ColorFormat::Unknown;
}

void DrawBuf::clear(const lv_area_t* area) {
  if (buf_) {
    lv_draw_buf_clear(buf_, area);
  }
}

void DrawBuf::set_flag(lv_image_flags_t flag) {
  if (buf_) {
    lv_draw_buf_set_flag(buf_, flag);
  }
}

void DrawBuf::clear_flag(lv_image_flags_t flag) {
  if (buf_) {
    lv_draw_buf_clear_flag(buf_, flag);
  }
}

void* DrawBuf::data() { return buf_ ? buf_->data : nullptr; }

const void* DrawBuf::data() const { return buf_ ? buf_->data : nullptr; }

size_t DrawBuf::data_size() const { return buf_ ? buf_->data_size : 0; }

void DrawBuf::swap_endianness() { swap_bytes(); }

void DrawBuf::swap_bytes() {
  if (!buf_ || !buf_->data) return;

  ColorFormat cf = format();
  if (cf == ColorFormat::RGB565) {
    lv_draw_sw_rgb565_swap(buf_->data, width() * height());
  } else if (cf == ColorFormat::ARGB8888 || cf == ColorFormat::XRGB8888) {
    uint32_t* p = reinterpret_cast<uint32_t*>(buf_->data);
    size_t count = (width() * height());
    for (size_t i = 0; i < count; ++i) {
      p[i] = __builtin_bswap32(p[i]);
    }
  } else {
    // Generic fallback for 16-bit
    if (LV_COLOR_FORMAT_GET_BPP(static_cast<lv_color_format_t>(cf)) == 16) {
      uint16_t* p = reinterpret_cast<uint16_t*>(buf_->data);
      size_t count = (width() * height());
      for (size_t i = 0; i < count; ++i) {
        p[i] = __builtin_bswap16(p[i]);
      }
    }
  }
}

void DrawBuf::invert_colors() {
  if (!buf_ || !buf_->data) return;
  uint8_t* p = static_cast<uint8_t*>(buf_->data);
  for (size_t i = 0; i < buf_->data_size; i++) {
    p[i] = ~p[i];
  }
}

void DrawBuf::premultiply() {
  if (buf_) {
    lv_draw_buf_premultiply(buf_);
  }
}

}  // namespace draw
}  // namespace lvgl
