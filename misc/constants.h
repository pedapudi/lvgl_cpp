#ifndef LVGL_CPP_MISC_CONSTANTS_H_
#define LVGL_CPP_MISC_CONSTANTS_H_

#include "lvgl.h"

namespace lvgl {

/**
 * @brief Namespace for size-related constants.
 */
namespace Size {

/**
 * @brief Special value to set height or width to content size.
 */
static constexpr int32_t Content = LV_SIZE_CONTENT;

/**
 * @brief Create a percentage value for coordinates or sizes.
 * @param x The percentage (0..100).
 * @return A special value representing the percentage.
 */
inline int32_t Percent(int32_t x) { return LV_PCT(x); }

}  // namespace Size

/**
 * @brief Namespace for radius-related constants.
 */
namespace Radius {

/**
 * @brief Special value for a very big radius to always draw as circular.
 */
static constexpr int32_t Circle = LV_RADIUS_CIRCLE;

}  // namespace Radius

/**
 * @brief Namespace for Grid layout constants.
 */
namespace Grid {

/**
 * @brief Template constants for grid columns and rows.
 */
namespace Template {

/**
 * @brief Special value to make the track fill the free space proportionally.
 * @param x The proportion factor.
 * @return A special track size.
 */
inline int32_t Fr(uint8_t x) { return LV_GRID_FR(x); }

/**
 * @brief Special value to make the track size fit its content.
 */
static constexpr int32_t Content = LV_GRID_CONTENT;

/**
 * @brief Special value to mark the last element of a template array.
 */
static constexpr int32_t Last = LV_GRID_TEMPLATE_LAST;

}  // namespace Template

}  // namespace Grid

}  // namespace lvgl

#endif  // LVGL_CPP_MISC_CONSTANTS_H_
