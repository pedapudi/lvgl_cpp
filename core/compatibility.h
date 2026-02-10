#ifndef LVGL_CPP_CORE_COMPATIBILITY_H_
#define LVGL_CPP_CORE_COMPATIBILITY_H_

#include "lvgl.h"

/**
 * @file compatibility.h
 * @brief Compatibility layer for different LVGL versions (v9.3 vs v9.4+).
 *
 * This header provides macro mappings and fallbacks to ensure lvgl_cpp
 * compiles against older or minimal LVGL configurations, such as those
 * used in ZMK (LVGL v9.3) or Zephyr.
 */

// ---------------------------------------------------------------------------
// Animation Constants (v9.3 -> v9.4 rename)
// ---------------------------------------------------------------------------
// LVGL v9.4 renamed LV_SCR_LOAD_ANIM_* to LV_SCREEN_LOAD_ANIM_*.
// If the new names are missing (v9.3), map them to the old ones.

// Only define these if we are on < v9.4 (e.g. v9.3 where names are LV_SCR_...)
#if LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR < 4
#ifndef LV_SCREEN_LOAD_ANIM_NONE
#define LV_SCREEN_LOAD_ANIM_NONE LV_SCR_LOAD_ANIM_NONE
#define LV_SCREEN_LOAD_ANIM_OVER_LEFT LV_SCR_LOAD_ANIM_OVER_LEFT
#define LV_SCREEN_LOAD_ANIM_OVER_RIGHT LV_SCR_LOAD_ANIM_OVER_RIGHT
#define LV_SCREEN_LOAD_ANIM_OVER_TOP LV_SCR_LOAD_ANIM_OVER_TOP
#define LV_SCREEN_LOAD_ANIM_OVER_BOTTOM LV_SCR_LOAD_ANIM_OVER_BOTTOM
#define LV_SCREEN_LOAD_ANIM_MOVE_LEFT LV_SCR_LOAD_ANIM_MOVE_LEFT
#define LV_SCREEN_LOAD_ANIM_MOVE_RIGHT LV_SCR_LOAD_ANIM_MOVE_RIGHT
#define LV_SCREEN_LOAD_ANIM_MOVE_TOP LV_SCR_LOAD_ANIM_MOVE_TOP
#define LV_SCREEN_LOAD_ANIM_MOVE_BOTTOM LV_SCR_LOAD_ANIM_MOVE_BOTTOM
#define LV_SCREEN_LOAD_ANIM_FADE_IN LV_SCR_LOAD_ANIM_FADE_IN
#define LV_SCREEN_LOAD_ANIM_FADE_ON LV_SCR_LOAD_ANIM_FADE_ON
#define LV_SCREEN_LOAD_ANIM_FADE_OUT LV_SCR_LOAD_ANIM_FADE_OUT
#define LV_SCREEN_LOAD_ANIM_OUT_LEFT LV_SCR_LOAD_ANIM_OUT_LEFT
#define LV_SCREEN_LOAD_ANIM_OUT_RIGHT LV_SCR_LOAD_ANIM_OUT_RIGHT
#define LV_SCREEN_LOAD_ANIM_OUT_TOP LV_SCR_LOAD_ANIM_OUT_TOP
#define LV_SCREEN_LOAD_ANIM_OUT_BOTTOM LV_SCR_LOAD_ANIM_OUT_BOTTOM
#endif
#endif

// ---------------------------------------------------------------------------
// Module Availability Checks
// ---------------------------------------------------------------------------

// Check for Observer module (often disabled in ZMK)
#ifndef LV_USE_OBSERVER
#define LV_USE_OBSERVER 0
#endif

// Check for Matrix transformation support
#ifndef LV_DRAW_TRANSFORM_USE_MATRIX
#define LV_DRAW_TRANSFORM_USE_MATRIX 0
#endif

// Check for Snapshot support
#ifndef LV_USE_SNAPSHOT
#define LV_USE_SNAPSHOT 0
#endif

// Check for File Explorer support
#ifndef LV_USE_FILE_EXPLORER
#define LV_USE_FILE_EXPLORER 0
#endif

// Check for FS API support
#ifndef LV_USE_FS_API
// Generally defined in lv_conf.h, but ensure safe default
#define LV_USE_FS_API 0
#endif

// ---------------------------------------------------------------------------
// Version-Specific Feature Flags
// ---------------------------------------------------------------------------

// Detect v9.4+ features based on specific marker macros or version check
// Since we don't have a granular patch version check for dev versions,
// we infer based on existence of new macros if possible, or assume 9.4 features
// unless explicitly disabled. However, for ZMK/v9.3, we know certain things
// fail.

// Helper macro to detect if we have the new FS helpers
// (heuristically defined; in a real scenario we might check LV_VERSION_CHECK)
#if LVGL_VERSION_MAJOR > 9 || \
    (LVGL_VERSION_MAJOR == 9 && LVGL_VERSION_MINOR >= 4)
#define LVGL_CPP_HAS_NEW_FS_HELPERS 1
#define LVGL_CPP_HAS_IMAGE_SIZE_HELPERS 1
#define LVGL_CPP_HAS_LABEL_VFORMAT 1
#define LVGL_CPP_HAS_INDEV_TIMESTAMP 1
#define LVGL_CPP_HAS_INDEV_GESTURE_ARRAY 1
#else
#define LVGL_CPP_HAS_NEW_FS_HELPERS 0
#define LVGL_CPP_HAS_IMAGE_SIZE_HELPERS 0
#define LVGL_CPP_HAS_LABEL_VFORMAT 0
#define LVGL_CPP_HAS_INDEV_TIMESTAMP 0
#define LVGL_CPP_HAS_INDEV_GESTURE_ARRAY 0
#endif

#endif  // LVGL_CPP_CORE_COMPATIBILITY_H_
