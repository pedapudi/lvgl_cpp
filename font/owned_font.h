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

  // TODO: Add load_tiny_ttf if TineTTF is enabled.

 private:
  /**
   * @brief Private constructor taking ownership of a raw pointer.
   * @param font Raw lv_font_t pointer created by LVGL.
   */
  explicit OwnedFont(lv_font_t* font);

  // Helper to safely delete the managed font
  void destroy();
};

}  // namespace lvgl

#endif  // LVGL_CPP_FONT_OWNED_FONT_H_
