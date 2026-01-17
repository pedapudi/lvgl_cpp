#ifndef LVGL_CPP_DRAW_IMAGE_DECODER_H_
#define LVGL_CPP_DRAW_IMAGE_DECODER_H_

#include <functional>

#include "lvgl.h"  // IWYU pragma: export

namespace lvgl {

class ImageDecoder {
 public:
  ImageDecoder();  // Registered automatically in constructor? Or pure wrapper?
                   // Custom decoder creation: lv_image_decoder_create()
  virtual ~ImageDecoder();

  // If wrapping an existing decoder
  explicit ImageDecoder(lv_image_decoder_t* decoder);

  // Configuration
  using InfoCallback = std::function<lv_result_t(
      lv_image_decoder_t*, lv_image_decoder_dsc_t*, lv_image_header_t*)>;
  using OpenCallback =
      std::function<lv_result_t(lv_image_decoder_t*, lv_image_decoder_dsc_t*)>;
  using GetAreaCallback =
      std::function<lv_result_t(lv_image_decoder_t*, lv_image_decoder_dsc_t*,
                                const lv_area_t*, lv_area_t*)>;
  using CloseCallback =
      std::function<void(lv_image_decoder_t*, lv_image_decoder_dsc_t*)>;

  ImageDecoder& set_info_cb(InfoCallback cb);
  ImageDecoder& set_open_cb(OpenCallback cb);
  ImageDecoder& set_get_area_cb(GetAreaCallback cb);
  ImageDecoder& set_close_cb(CloseCallback cb);

  /**
   * @brief Release ownership of the decoder.
   */
  lv_image_decoder_t* release();

  lv_image_decoder_t* raw() const { return decoder_; }

  // Internal accessors for static shims
  const InfoCallback& get_info_cb() const { return info_cb_; }
  const OpenCallback& get_open_cb() const { return open_cb_; }
  const GetAreaCallback& get_get_area_cb() const { return get_area_cb_; }
  const CloseCallback& get_close_cb() const { return close_cb_; }

 protected:
  lv_image_decoder_t* decoder_;
  bool owned_ = false;

  InfoCallback info_cb_;
  OpenCallback open_cb_;
  GetAreaCallback get_area_cb_;
  CloseCallback close_cb_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_DRAW_IMAGE_DECODER_H_
