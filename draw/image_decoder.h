#ifndef LVGL_CPP_DRAW_IMAGE_DECODER_H_
#define LVGL_CPP_DRAW_IMAGE_DECODER_H_

#include "lvgl.h" // IWYU pragma: export

namespace lvgl {

class ImageDecoder {
public:
  ImageDecoder(); // Registered automatically in constructor? Or pure wrapper?
                  // Custom decoder creation: lv_image_decoder_create()
  virtual ~ImageDecoder();

  // If wrapping an existing decoder
  explicit ImageDecoder(lv_image_decoder_t* decoder);

  // Configuration
  void set_info_cb(lv_image_decoder_info_f_t cb);
  void set_open_cb(lv_image_decoder_open_f_t cb);
  void set_get_area_cb(lv_image_decoder_get_area_cb_t cb);
  void set_close_cb(lv_image_decoder_close_f_t cb);

  /**
   * @brief Release ownership of the decoder.
   */
  lv_image_decoder_t* release();

protected:
  lv_image_decoder_t* decoder_;
  bool owned_ = false;
};

} // namespace lvgl

#endif // LVGL_CPP_DRAW_IMAGE_DECODER_H_
