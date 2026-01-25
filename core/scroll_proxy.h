#pragma once

#include "../misc/enums.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Transient proxy object for scrolling operations on a widget.
 *
 * This class is returned by `Object::scroll()` and allows controlling scroll
 * behavior directly on the object using a fluent API.
 */
class ScrollProxy {
 public:
  explicit ScrollProxy(lv_obj_t* obj) : obj_(obj) {}

  /**
   * @brief Scroll to a specific position.
   * @param x X coordinate.
   * @param y Y coordinate.
   * @param anim_en Animation enable/disable.
   */
  ScrollProxy& to(int32_t x, int32_t y, AnimEnable anim_en) {
    lv_obj_scroll_to(obj_, x, y, static_cast<lv_anim_enable_t>(anim_en));
    return *this;
  }

  /**
   * @brief Scroll to a specific position (X only).
   * @param x X coordinate.
   * @param anim_en Animation enable/disable.
   */
  ScrollProxy& to_x(int32_t x, AnimEnable anim_en) {
    lv_obj_scroll_to_x(obj_, x, static_cast<lv_anim_enable_t>(anim_en));
    return *this;
  }

  /**
   * @brief Scroll to a specific position (Y only).
   * @param y Y coordinate.
   * @param anim_en Animation enable/disable.
   */
  ScrollProxy& to_y(int32_t y, AnimEnable anim_en) {
    lv_obj_scroll_to_y(obj_, y, static_cast<lv_anim_enable_t>(anim_en));
    return *this;
  }

  /**
   * @brief Scroll by a specific amount.
   * @param dx Change in X.
   * @param dy Change in Y.
   * @param anim_en Animation enable/disable.
   */
  ScrollProxy& by(int32_t dx, int32_t dy, AnimEnable anim_en) {
    lv_obj_scroll_by(obj_, dx, dy, static_cast<lv_anim_enable_t>(anim_en));
    return *this;
  }

  /**
   * @brief Scroll the object into view.
   * @param anim_en Animation enable/disable.
   */
  ScrollProxy& to_view(AnimEnable anim_en) {
    lv_obj_scroll_to_view(obj_, static_cast<lv_anim_enable_t>(anim_en));
    return *this;
  }

  /**
   * @brief Set the scrollbar mode.
   * @param mode The new mode.
   */
  ScrollProxy& mode(ScrollbarMode mode) {
    lv_obj_set_scrollbar_mode(obj_, static_cast<lv_scrollbar_mode_t>(mode));
    return *this;
  }

  /**
   * @brief Set the scroll snap behavior.
   * @param snap_x Snap behavior in X.
   * @param snap_y Snap behavior in Y.
   */
  ScrollProxy& snap(ScrollSnap snap_x, ScrollSnap snap_y) {
    lv_obj_set_scroll_snap_x(obj_, static_cast<lv_scroll_snap_t>(snap_x));
    lv_obj_set_scroll_snap_y(obj_, static_cast<lv_scroll_snap_t>(snap_y));
    return *this;
  }

  /**
   * @brief Set the scroll direction.
   * @param dir allowed scroll direction.
   */
  ScrollProxy& dir(Dir dir) {
    lv_obj_set_scroll_dir(obj_, static_cast<lv_dir_t>(dir));
    return *this;
  }

 private:
  lv_obj_t* obj_;
};

}  // namespace lvgl
