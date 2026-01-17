#include "image_decoder.h"

#include <iostream>
#include <unordered_map>

namespace {
// Map to retrieve C++ instance from C pointer, since user_data is not publicly
// accessible.
std::unordered_map<lv_image_decoder_t*, lvgl::ImageDecoder*> g_decoder_map;

lv_result_t info_cb_shim(lv_image_decoder_t* decoder,
                         lv_image_decoder_dsc_t* dsc,
                         lv_image_header_t* header) {
  std::cout << "Shim called for decoder: " << decoder << std::endl;
  auto it = g_decoder_map.find(decoder);
  if (it != g_decoder_map.end()) {
    std::cout << "Found in map. Has info_cb: "
              << (bool)it->second->get_info_cb() << std::endl;
    if (it->second->get_info_cb()) {
      return it->second->get_info_cb()(decoder, dsc, header);
    }
  } else {
    std::cout << "Not found in map!" << std::endl;
  }
  return LV_RESULT_INVALID;  // Or pass through? Usually we return invalid if
                             // can't handle.
}

lv_result_t open_cb_shim(lv_image_decoder_t* decoder,
                         lv_image_decoder_dsc_t* dsc) {
  auto it = g_decoder_map.find(decoder);
  if (it != g_decoder_map.end() && it->second->get_open_cb()) {
    return it->second->get_open_cb()(decoder, dsc);
  }
  return LV_RESULT_INVALID;
}

lv_result_t get_area_cb_shim(lv_image_decoder_t* decoder,
                             lv_image_decoder_dsc_t* dsc,
                             const lv_area_t* full_area,
                             lv_area_t* decoded_area) {
  auto it = g_decoder_map.find(decoder);
  if (it != g_decoder_map.end() && it->second->get_get_area_cb()) {
    return it->second->get_get_area_cb()(decoder, dsc, full_area, decoded_area);
  }
  return LV_RESULT_INVALID;
}

void close_cb_shim(lv_image_decoder_t* decoder, lv_image_decoder_dsc_t* dsc) {
  auto it = g_decoder_map.find(decoder);
  if (it != g_decoder_map.end() && it->second->get_close_cb()) {
    it->second->get_close_cb()(decoder, dsc);
  }
}

}  // namespace

namespace lvgl {

ImageDecoder::ImageDecoder() {
  decoder_ = lv_image_decoder_create();
  owned_ = true;
  g_decoder_map[decoder_] = this;
}

ImageDecoder::ImageDecoder(lv_image_decoder_t* decoder)
    : decoder_(decoder), owned_(false) {
  // If wrapping an existing one, we might interfere if we set callbacks?
  // But for now, we register it.
  g_decoder_map[decoder_] = this;
}

ImageDecoder::~ImageDecoder() {
  g_decoder_map.erase(decoder_);
  if (owned_ && decoder_) {
    lv_image_decoder_delete(decoder_);
  }
}

ImageDecoder& ImageDecoder::set_info_cb(InfoCallback cb) {
  info_cb_ = cb;
  // TODO: Install shim if needed or if user data handling is implemented
  return *this;
}

ImageDecoder& ImageDecoder::set_open_cb(OpenCallback cb) {
  open_cb_ = cb;
  // TODO: Install shim if needed or if user data handling is implemented
  return *this;
}

ImageDecoder& ImageDecoder::set_get_area_cb(GetAreaCallback cb) {
  get_area_cb_ = cb;
  // TODO: Install shim if needed or if user data handling is implemented
  return *this;
}

ImageDecoder& ImageDecoder::set_close_cb(CloseCallback cb) {
  close_cb_ = cb;
  // TODO: Install shim if needed or if user data handling is implemented
  return *this;
}

lv_image_decoder_t* ImageDecoder::release() {
  g_decoder_map.erase(decoder_);  // Stop tracking
  lv_image_decoder_t* ptr = decoder_;
  owned_ = false;
  return ptr;
}

}  // namespace lvgl
