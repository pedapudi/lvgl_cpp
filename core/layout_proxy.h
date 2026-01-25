#pragma once

#include "../misc/enums.h"
#include "lvgl.h"

namespace lvgl {

/**
 * @brief Transient proxy object for setting layout properties on a widget.
 *
 * This class is returned by `Object::layout()` and allows setting flex and grid
 * properties directly on the object using a fluent API.
 */
class LayoutProxy {
 public:
  explicit LayoutProxy(lv_obj_t* obj) : obj_(obj) {}

  // =========================================================================
  // Flex Layout
  // =========================================================================

  /**
   * @brief Set the flex flow direction.
   * @param flow The flow direction (e.g. FlexFlow::Row).
   */
  LayoutProxy& flex_flow(FlexFlow flow) {
    lv_obj_set_flex_flow(obj_, static_cast<lv_flex_flow_t>(flow));
    return *this;
  }

  /**
   * @brief Set the flex alignment.
   * @param main_place Alignment on main axis.
   * @param cross_place Alignment on cross axis.
   * @param track_cross_place Alignment of tracks.
   */
  LayoutProxy& flex_align(FlexAlign main_place, FlexAlign cross_place,
                          FlexAlign track_cross_place) {
    lv_obj_set_flex_align(obj_, static_cast<lv_flex_align_t>(main_place),
                          static_cast<lv_flex_align_t>(cross_place),
                          static_cast<lv_flex_align_t>(track_cross_place));
    return *this;
  }

  /**
   * @brief Set the flex grow factor.
   * @param grow Grow factor (0 to disable).
   */
  LayoutProxy& flex_grow(uint8_t grow) {
    lv_obj_set_style_flex_grow(obj_, grow, LV_PART_MAIN);
    return *this;
  }

  // =========================================================================
  // Grid Layout
  // =========================================================================

  /**
   * @brief Set the grid descriptor arrays.
   * @param col_dsc Array of column descriptors (must be static/global).
   * @param row_dsc Array of row descriptors (must be static/global).
   */
  LayoutProxy& grid_dsc(const int32_t col_dsc[], const int32_t row_dsc[]) {
    lv_obj_set_grid_dsc_array(obj_, col_dsc, row_dsc);
    return *this;
  }

  /**
   * @brief Set the grid alignment.
   * @param column_align Alignment of columns.
   * @param row_align Alignment of rows.
   */
  LayoutProxy& grid_align(GridAlign column_align, GridAlign row_align) {
    lv_obj_set_grid_align(obj_, static_cast<lv_grid_align_t>(column_align),
                          static_cast<lv_grid_align_t>(row_align));
    return *this;
  }

  /**
   * @brief Set the grid cell placement for this object.
   * @param column_align Alignment in column.
   * @param col_pos Column index.
   * @param col_span Column span.
   * @param row_align Alignment in row.
   * @param row_pos Row index.
   * @param row_span Row span.
   */
  LayoutProxy& grid_cell(GridAlign column_align, int32_t col_pos,
                         int32_t col_span, GridAlign row_align, int32_t row_pos,
                         int32_t row_span) {
    lv_obj_set_grid_cell(
        obj_, static_cast<lv_grid_align_t>(column_align), col_pos, col_span,
        static_cast<lv_grid_align_t>(row_align), row_pos, row_span);
    return *this;
  }

  /**
   * @brief Update the layout of the object.
   */
  LayoutProxy& update() {
    lv_obj_update_layout(obj_);
    return *this;
  }

 private:
  lv_obj_t* obj_;
};

}  // namespace lvgl
