#ifndef LVGL_CPP_DISPLAY_DISPLAY_H_
#define LVGL_CPP_DISPLAY_DISPLAY_H_

#include <functional>
#include <vector>

#include "../core/object.h"
#include "lvgl.h"

/**
 * @file display.h
 * @brief User Guide:
 * The `Display` class wraps an `lv_display_t` and manages its resolution,
 * rotation, buffers, and active screens.
 *
 * Key Features:
 * - **Lifecycle**: Can wrap existing displays or create new ones.
 * - **Buffer Management**: Can automatically allocate or accept user-provided
 * buffers.
 * - **Screens & Layers**: Provides quick access to the active screen and system
 * layers as `Object` wrappers.
 * - **Rotation & DPI**: Easy configuration of physical display properties.
 */

namespace lvgl {

namespace draw {
class DrawBuf;
}

/**
 * @brief Manager for a physical or virtual display.
 */
class Display {
 public:
  using FlushCallback = std::function<void(Display* disp, const lv_area_t* area,
                                           uint8_t* px_map)>;
  using FlushWaitCallback = std::function<void(Display* disp)>;

  // Usually displays are created by drivers, but we can wrap them.
  explicit Display(lv_display_t* disp);
  virtual ~Display() = default;

  // Non-copyable
  Display(const Display&) = delete;
  Display& operator=(const Display&) = delete;

  // Moveable
  Display(Display&& other) noexcept;
  Display& operator=(Display&& other) noexcept;

  /**
   * @brief Create a new display.
   * @param hor_res Horizontal resolution.
   * @param ver_res Vertical resolution.
   * @return Display wrapper.
   */
  static Display create(int32_t hor_res, int32_t ver_res);

  /**
   * @brief Delete the display.
   */
  void delete_display();

  /**
   * @brief Get the default display.
   * @return Pointer to the default display.
   */
  static Display* get_default();

  /**
   * @brief Set the default display.
   * @param disp Pointer to the display.
   */
  static void set_default(Display* disp);

  /**
   * @brief Get the next display in the list.
   * @param prev Previous display.
   * @return Pointer to the next display.
   */
  static Display* get_next(Display* prev);

  // Resolution & Physical
  /**
   * @brief Set display resolution.
   * @param hor_res Horizontal resolution.
   * @param ver_res Vertical resolution.
   */
  void set_resolution(int32_t hor_res, int32_t ver_res);

  /**
   * @brief Set physical resolution.
   * @param hor_res Horizontal resolution.
   * @param ver_res Vertical resolution.
   */
  void set_physical_resolution(int32_t hor_res, int32_t ver_res);

  /**
   * @brief Set display offset.
   * @param x X offset.
   * @param y Y offset.
   */
  void set_offset(int32_t x, int32_t y);

  /** @brief Get horizontal resolution. */
  int32_t get_horizontal_resolution() const;
  /** @brief Get vertical resolution. */
  int32_t get_vertical_resolution() const;
  /** @brief Get physical horizontal resolution. */
  int32_t get_physical_horizontal_resolution() const;
  /** @brief Get physical vertical resolution. */
  int32_t get_physical_vertical_resolution() const;
  /** @brief Get X offset. */
  int32_t get_offset_x() const;
  /** @brief Get Y offset. */
  int32_t get_offset_y() const;
  /** @brief Get original horizontal resolution. */
  int32_t get_original_horizontal_resolution() const;
  /** @brief Get original vertical resolution. */
  int32_t get_original_vertical_resolution() const;

  /**
   * @brief Display rotation values.
   */
  enum class Rotation {
    ROT_0 = LV_DISPLAY_ROTATION_0,
    ROT_90 = LV_DISPLAY_ROTATION_90,
    ROT_180 = LV_DISPLAY_ROTATION_180,
    ROT_270 = LV_DISPLAY_ROTATION_270
  };

  /**
   * @brief Set display rotation.
   * @param rotation Rotation value.
   */
  void set_rotation(Rotation rotation);

  /** @brief Get current rotation. */
  Rotation get_rotation() const;

  /** @brief Enable or disable software rotation matrix. */
  void set_matrix_rotation(bool enable);

  /** @brief Check if software rotation matrix is enabled. */
  bool get_matrix_rotation() const;

  // DPI
  /** @brief Set display DPI. */
  void set_dpi(int32_t dpi);
  /** @brief Get display DPI. */
  int32_t get_dpi() const;

  /**
   * @brief Display render modes.
   */
  enum class RenderMode : uint8_t {
    Partial = LV_DISPLAY_RENDER_MODE_PARTIAL,
    Direct = LV_DISPLAY_RENDER_MODE_DIRECT,
    Full = LV_DISPLAY_RENDER_MODE_FULL,
  };

  /**
   * @brief Screen load animations.
   */
  enum class LoadAnim : uint8_t {
    None = LV_SCREEN_LOAD_ANIM_NONE,
    FadeIn = LV_SCREEN_LOAD_ANIM_FADE_IN,
    FadeOn = LV_SCREEN_LOAD_ANIM_FADE_ON,
    FadeOut = LV_SCREEN_LOAD_ANIM_FADE_OUT,
    OverLeft = LV_SCREEN_LOAD_ANIM_OVER_LEFT,
    OverRight = LV_SCREEN_LOAD_ANIM_OVER_RIGHT,
    OverTop = LV_SCREEN_LOAD_ANIM_OVER_TOP,
    OverBottom = LV_SCREEN_LOAD_ANIM_OVER_BOTTOM,
    MoveLeft = LV_SCREEN_LOAD_ANIM_MOVE_LEFT,
    MoveRight = LV_SCREEN_LOAD_ANIM_MOVE_RIGHT,
    MoveTop = LV_SCREEN_LOAD_ANIM_MOVE_TOP,
    MoveBottom = LV_SCREEN_LOAD_ANIM_MOVE_BOTTOM,
    OutLeft = LV_SCREEN_LOAD_ANIM_OUT_LEFT,
    OutRight = LV_SCREEN_LOAD_ANIM_OUT_RIGHT,
    OutTop = LV_SCREEN_LOAD_ANIM_OUT_TOP,
    OutBottom = LV_SCREEN_LOAD_ANIM_OUT_BOTTOM,
  };

  // Buffers & Rendering
  /**
   * @brief Set drawing buffers for the display.
   * @param buf1 First buffer.
   * @param buf2 Second buffer (optional, for double buffering).
   * @param buf_size Buffer size in bytes.
   * @param render_mode Rendering mode (Partial, Direct, Full).
   */
  void set_buffers(void* buf1, void* buf2, uint32_t buf_size,
                   RenderMode render_mode);

  /**
   * @brief Set drawing buffers with custom stride.
   */
  void set_buffers_with_stride(void* buf1, void* buf2, uint32_t buf_size,
                               uint32_t stride, RenderMode render_mode);

  /** @brief Set draw buffers using lv_draw_buf_t objects. */
  void set_draw_buffers(const draw::DrawBuf& buf1,
                        const draw::DrawBuf* buf2 = nullptr);

  /** @brief Set a third draw buffer for triple buffering. */
  void set_3rd_draw_buffer(const draw::DrawBuf& buf3);

  /** @brief Set the rendering mode. */
  void set_render_mode(RenderMode render_mode);

  /** @brief Set the number of tiles (for tiling renderers). */
  void set_tile_cnt(uint32_t tile_cnt);

  /** @brief Enable or disable antialiasing. */
  void set_antialiasing(bool en);

  /** @brief Get tile count. */
  uint32_t get_tile_cnt() const;
  /** @brief Check if antialiasing is enabled. */
  bool get_antialiasing() const;
  /** @brief Check if double buffering is enabled. */
  bool is_double_buffered() const;
  /** @brief Get required draw buffer size. */
  uint32_t get_draw_buf_size() const;
  /** @brief Get required draw buffer size for a specific area. */
  uint32_t get_invalidated_draw_buf_size(uint32_t width, uint32_t height) const;

  // Color Format
  /** @brief Set the color format. */
  void set_color_format(ColorFormat color_format);

  /** @brief Get the color format. */
  ColorFormat get_color_format() const;

  // Callbacks
  /** @brief Set the flush callback. */
  void set_flush_cb(FlushCallback cb);
  /** @brief Set the flush wait callback. */
  void set_flush_wait_cb(FlushWaitCallback cb);
  /** @brief Indicate that flushing is ready. */
  void flush_ready();
  /** @brief Check if the current flush is the last one in a sequence. */
  bool flush_is_last();

  // Screens & Layers
  /** @brief Get raw handle to active screen. */
  lv_obj_t* get_screen_active();
  /** @brief Get raw handle to previous screen. */
  lv_obj_t* get_screen_prev();
  /** @brief Get raw handle to screen being loaded. */
  lv_obj_t* get_screen_loading();
  /** @brief Get raw handle to top layer. */
  lv_obj_t* get_layer_top();
  /** @brief Get raw handle to system layer. */
  lv_obj_t* get_layer_sys();
  /** @brief Get raw handle to bottom layer. */
  lv_obj_t* get_layer_bottom();

  /** @brief Get active screen as Object. */
  Object screen_active() {
    return Object(get_screen_active(), Object::Ownership::Unmanaged);
  }
  /** @brief Get previous screen as Object. */
  Object screen_prev() {
    return Object(get_screen_prev(), Object::Ownership::Unmanaged);
  }
  /** @brief Get loading screen as Object. */
  Object screen_loading() {
    return Object(get_screen_loading(), Object::Ownership::Unmanaged);
  }
  /** @brief Get top layer as Object. */
  Object layer_top() {
    return Object(get_layer_top(), Object::Ownership::Unmanaged);
  }
  /** @brief Get system layer as Object. */
  Object layer_sys() {
    return Object(get_layer_sys(), Object::Ownership::Unmanaged);
  }
  /** @brief Get bottom layer as Object. */
  Object layer_bottom() {
    return Object(get_layer_bottom(), Object::Ownership::Unmanaged);
  }

  /** @brief Load a screen. */
  void load_screen(Object& scr);

  /** @brief Load a screen with animation. */
  void load_screen_anim(Object& scr, LoadAnim anim_type, uint32_t time,
                        uint32_t delay, bool auto_del);

  // Theme
  /** @brief Set display theme. */
  void set_theme(lv_theme_t* th);
  /** @brief Get display theme. */
  lv_theme_t* get_theme();

  // Activity & Invalidation
  /** @brief Get time since last activity. */
  uint32_t get_inactive_time() const;
  /** @brief Trigger user activity. */
  void trigger_activity();
  /** @brief Enable or disable invalidation. */
  void enable_invalidation(bool en);
  /** @brief Check if invalidation is enabled. */
  bool is_invalidation_enabled() const;

  // Timer
  /** @brief Delete the refresh timer. */
  void delete_refr_timer();

  /** @brief Get underlying LVGL handle. */
  lv_display_t* raw() const { return disp_; }

  // Utilities
  void clear_active_screen();
  void auto_configure_buffers(RenderMode mode = RenderMode::Partial,
                              bool double_buffer = false);

 private:
  lv_display_t* disp_;
  std::vector<uint8_t> buf1_;
  std::vector<uint8_t> buf2_;
};

}  // namespace lvgl

#endif  // LVGL_CPP_DISPLAY_DISPLAY_H_
