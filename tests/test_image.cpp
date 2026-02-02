#include <iostream>

#include "../core/object.h"
#include "../display/display.h"
#include "../draw/draw_buf.h"
#include "../draw/image_descriptor.h"
#include "../widgets/image.h"
#include "../lvgl_cpp.h"
#include "src/draw/sw/lv_draw_sw_utils.h"

void test_image_path() {
  std::cout << "Testing Image Path..." << std::endl;
  lvgl::Object screen(lv_screen_active(), lvgl::Object::Ownership::Unmanaged);
  lvgl::Image img(screen);

  // Test with a symbol string (treated same as file path for copy/safety)
  std::string path = LV_SYMBOL_OK;
  img.set_src(path);

  // Verify we can get it back
  const void* src = img.get_src();
  if (src) {
    lv_image_src_t type = lv_image_src_get_type(src);
    if (type == LV_IMAGE_SRC_SYMBOL) {
      std::cout << "PASS: Image source set as symbol." << std::endl;
    } else {
      std::cerr << "FAIL: Image source type mismatch: " << type << std::endl;
      exit(1);
    }
  } else {
    std::cerr << "FAIL: get_src returned null." << std::endl;
    exit(1);
  }
}

void test_image_descriptor() {
  // ... (previous content)
}

void test_draw_buf_helpers() {
  std::cout << "Testing DrawBuf Helpers..." << std::endl;

  uint32_t w = 2;
  uint32_t h = 2;
  lvgl::draw::DrawBuf buf(w, h, lvgl::ColorFormat::RGB565);

  // Fill with known value
  uint16_t* p = static_cast<uint16_t*>(buf.data());
  uint16_t original_color = 0xABCD;
  p[0] = original_color;

  // Swap
  buf.swap_endianness();

  uint16_t swapped = p[0];
  uint16_t expected = (original_color >> 8) | (original_color << 8);

  if (swapped == expected) {
    std::cout << "PASS: RGB565 swap_endianness (0x" << std::hex
              << original_color << " -> 0x" << swapped << ")" << std::dec
              << std::endl;
  } else {
    std::cerr << "FAIL: RGB565 swap_endianness. Expected: 0x" << std::hex
              << expected << ", Got: 0x" << swapped << std::dec << std::endl;
    exit(1);
  }

  // Test 32-bit
  lvgl::draw::DrawBuf buf32(w, h, lvgl::ColorFormat::ARGB8888);
  uint32_t* p32 = static_cast<uint32_t*>(buf32.data());
  uint32_t original_color32 = 0x11223344;
  p32[0] = original_color32;

  buf32.swap_endianness();

  uint32_t swapped32 = p32[0];
  uint32_t expected32 = __builtin_bswap32(original_color32);

  if (swapped32 == expected32) {
    std::cout << "PASS: ARGB8888 swap_endianness (0x" << std::hex
              << original_color32 << " -> 0x" << swapped32 << ")" << std::dec
              << std::endl;
  } else {
    std::cerr << "FAIL: ARGB8888 swap_endianness. Expected: 0x" << std::hex
              << expected32 << ", Got: 0x" << swapped32 << std::dec
              << std::endl;
    exit(1);
  }
}

int main() {
  lv_init();
  lvgl::Display display = lvgl::Display::create(800, 480);

  test_image_path();
  test_image_descriptor();
  test_draw_buf_helpers();

  return 0;
}
