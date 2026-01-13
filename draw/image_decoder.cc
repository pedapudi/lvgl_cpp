#include "image_decoder.h"

namespace lvgl {

ImageDecoder::ImageDecoder() {
  decoder_ = lv_image_decoder_create();
  owned_ = true;
}

ImageDecoder::ImageDecoder(lv_image_decoder_t* decoder)
    : decoder_(decoder), owned_(false) {}

ImageDecoder::~ImageDecoder() {
  if (owned_ && decoder_) {
    lv_image_decoder_delete(decoder_);
  }
}

void ImageDecoder::set_info_cb(lv_image_decoder_info_f_t cb) {
  if (decoder_)
    lv_image_decoder_set_info_cb(decoder_, cb);
}

void ImageDecoder::set_open_cb(lv_image_decoder_open_f_t cb) {
  if (decoder_)
    lv_image_decoder_set_open_cb(decoder_, cb);
}

void ImageDecoder::set_get_area_cb(lv_image_decoder_get_area_cb_t cb) {
  if (decoder_)
    lv_image_decoder_set_get_area_cb(decoder_, cb);
}

void ImageDecoder::set_close_cb(lv_image_decoder_close_f_t cb) {
  if (decoder_)
    lv_image_decoder_set_close_cb(decoder_, cb);
}

lv_image_decoder_t* ImageDecoder::release() {
  lv_image_decoder_t* ptr = decoder_;
  owned_ = false;
  return ptr;
}

} // namespace lvgl
