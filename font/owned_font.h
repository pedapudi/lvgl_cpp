#ifndef LVGL_CPP_FONT_OWNED_FONT_H_
#define LVGL_CPP_FONT_OWNED_FONT_H_

#include <string>

#include "font.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief RAII wrapper for an owning lv_font_t.
 * Manages the lifecycle of runtime-loaded fonts (e.g. from .bin or .ttf).
 */
class OwnedFont : public Font {
 public:
  /**
   * @brief Construct an empty/invalid OwnedFont.
   */
  OwnedFont();

  /**
   * @brief Move constructor.
   * Transfer ownership from other.
   */
  OwnedFont(OwnedFont&& other) noexcept;

  /**
   * @brief Move assignment.
   */
  OwnedFont& operator=(OwnedFont&& other) noexcept;

  // Non-copyable
  OwnedFont(const OwnedFont&) = delete;
  OwnedFont& operator=(const OwnedFont&) = delete;

  /**
   * @brief Destructor.
   * Destroys the font via lv_font_delete.
   */
  ~OwnedFont();

  /**
   * @brief Load a binary font from the filesystem.
   * Wrapper for lv_binfont_create.
   * @param path File path to the .bin font file.
   * @return OwnedFont instance. Check is_valid() for success.
   */
  static OwnedFont load_bin(const std::string& path);

  /**
   * @brief Create a tiny_ttf font from data.
   * @param data The font data.
   * @param data_size The size of the font data.
   * @param font_size The font size in pixel.
   * @return OwnedFont instance. Check is_valid() for success.
   */
  static OwnedFont load_tiny_ttf(const void* data, size_t data_size,
                                 int32_t font_size);

 private:
  enum class FontType { None, Binary, TinyTTF };

  /**
   * @brief Private constructor taking ownership of a raw pointer.
   * @param font Raw lv_font_t pointer created by LVGL.
   * @param type The type of font (used for destruction).
   */
  explicit OwnedFont(lv_font_t* font, FontType type = FontType::None);

  // Helper to safely delete the managed font
  void destroy();

  FontType type_ = FontType::None;
};

}  // namespace lvgl

#endif  // LVGL_CPP_FONT_OWNED_FONT_H_
