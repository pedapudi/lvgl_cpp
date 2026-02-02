#pragma once

#include "../core/widget.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Wrapper for LVGL Screen objects.
 * Screens are special objects that have no parent.
 */
class Screen : public Widget<Screen> {
 public:
  /**
   * @brief Create a new Screen.
   * Calls lv_obj_create(NULL).
   */
  Screen();

  /**
   * @brief Create a wrapper for an existing screen object.
   * @param obj The raw lv_obj_t pointer (must be a screen).
   * @param ownership Ownership policy.
   */
  explicit Screen(lv_obj_t* obj, Ownership ownership = Ownership::Default);

  /**
   * @brief Load this screen immediately.
   */
  void load();

  /**
   * @brief Load this screen with an animation.
   * @param anim_type The animation type (e.g., LV_SCR_LOAD_ANIM_FADE_ON).
   * @param time Duration of the animation in ms.
   * @param delay Delay before starting the animation in ms.
   * @param auto_del If true, automatically delete the old screen when finished.
   */
  void load_anim(ScreenLoadAnim anim_type, uint32_t time, uint32_t delay,
                 bool auto_del = false);

  /**
   * @brief Get the currently active screen.
   * @return Wrapper for lv_screen_active().
   */
  static Screen active();

  /**
   * @brief Get the top layer.
   * @return Wrapper for lv_layer_top().
   */
  static Screen top();

  /**
   * @brief Get the system layer.
   * @return Wrapper for lv_layer_sys().
   */
  static Screen sys();
};

}  // namespace lvgl
